///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           cms_client_manager.cpp
/// @brief          业务服务器管理端
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月27日
/// @Description
/// @History
///************************************************************
#include "cms_client_manager.h"
#include "module_config_collection.h"
#include "../../protoc/protocol_proc_manager.h"
#include "../../../module_data_center.h"
#include "../bss/bss_client_manager.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace jsbn;
using namespace jsbn::protoc;

CmsClientManager& CmsClientManager::Instance()
{
    static CmsClientManager instance;
    return instance;
}

CmsClientManager::CmsClientManager():_pServerWorker(nullptr),
    _client_mutex(RWLock::Create()),
    _cms_client(nullptr)
{}

CmsClientManager::~CmsClientManager()
{
    Stop();
    delete _client_mutex;
}

int CmsClientManager::Start()
{
    _pServerWorker = new(std::nothrow) ServerWorker(SYS_CONFIG->get_module_config().host_ip,
                                                    SYS_CONFIG->get_module_config().cms_service_listen_port);
    if (nullptr == _pServerWorker)
    {
        return FUNC_FAILED;
    }

    SignalAccept.connect(this, &CmsClientManager::Accept);
    SignalRecvData.connect(this, &CmsClientManager::RecvData);
    SignalEvent.connect(this, &CmsClientManager::Event);

    if (!_pServerWorker->StartWork(this))
    {
        LOG(ERROR)<<"服务器监听启动失败";
        return FUNC_FAILED;
    }

    return FUNC_SUCCESS;
}

void CmsClientManager::Stop()
{
    if (_pServerWorker)
    {
        _pServerWorker->StopWork();
        delete _pServerWorker;
        _pServerWorker = nullptr;
    }

    {
        WriteLockScoped wls(*_client_mutex);
        if(_cms_client)
        {
            _cms_client->StopWork();
            delete _cms_client;
            _cms_client = nullptr;
        }
    }
}

int CmsClientManager::SendData(const sSendDataPage_ptr& pSend)
{
    ReadLockScoped rls(*_client_mutex);
    if(_cms_client)
    {
        return _cms_client->SendData(pSend->send_buf, pSend->send_len);
    }

    return FUNC_FAILED;
}

int CmsClientManager::AddClient(unsigned short seq, PassiveTCPClient* p_client)
{
    WriteLockScoped wls(*_client_mutex);
    if(_cms_client)
    {
        LOG(ERROR)<<"已经有CMS客户端存在，不接收新的CMS客户端";
        return FUNC_FAILED;
    }

    _cms_client = p_client;

    return FUNC_SUCCESS;
}

void CmsClientManager::DelClient(unsigned short seq)
{
    WriteLockScoped wls(*_client_mutex);
    if (_cms_client)
    {
        _cms_client->StopWork();
        delete _cms_client;
        _cms_client = nullptr;
    }
}

void CmsClientManager::RecvData(unsigned short seq, const unsigned char* buf, PacketLength len)
{
    // 解析数据协议
    sProtocolData_ptr prt = ProtocolProcManager::ParseProtocol(buf, len);
    if (nullptr == prt)
    {
        LOG(ERROR)<<"协议解析失败，关闭连接.";
        DelClient(seq);
        return;
    }

    // 标记是那个连接收到的数据，便于业务处理完以后应答
    prt->sock_handle = seq;
    switch(prt->command_id)
    {
        case jsbn::protoc::CommandID::Heart_Beat://心跳
        {
            sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();

            pSend->sock_handle = prt->sock_handle;
            pSend->Copy(buf, len);

            SendData(pSend);

            return;
        }
        case jsbn::protoc::CommandID::Data_Relay_Req:// 数据转发请求
        {
            TDataRelayReq* pData = (TDataRelayReq*)prt.get();
            sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
            pSend->sock_handle = prt->sock_handle;
            pSend->Copy(pData->msg.c_str(), pData->msg.length());

            switch (pData->dst_srv_type)
            {
                case jsbn::protoc::ServiceTpye::ST_BSS:
                {
                    if(BssClientManager::Instance().SendData(pData->dst_city_id, pSend) != FUNC_SUCCESS)
                    {
                        LOG(ERROR)<<"转发数据失败,BSS客户端不在线";

                        std::string response;
                        jsbn::protoc::NetProtocol pc;

                        // 转发应答
                        pc.set_commandid(jsbn::protoc::CommandID::Data_Relay_Rsp);
                        pc.mutable_datarelayrsp()->set_srcsrvtype(pData->src_srv_type);
                        pc.mutable_datarelayrsp()->set_dstsrvtype(pData->dst_srv_type);
                        pc.mutable_datarelayrsp()->set_dstcityid(pData->dst_city_id);
                        pc.mutable_datarelayrsp()->set_result(-1);
                        pc.mutable_datarelayrsp()->set_error_description("转发数据失败,CMS客户端不在线");

                        pc.SerializeToString(&response);

                        sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
                        pSend->sock_handle = prt->sock_handle;
                        pSend->Copy(response.c_str(), response.length());

                        SendData(pSend);
                    }
                    else
                    {
                        LOG(DEBUG)<<"转发数据成功,目的服务器:BSS,CITY_ID:"<<pData->dst_city_id;
                    }

                    break;
                }
                default:
                {
                    LOG(ERROR)<<"CMS无效的转发服务器类型.["<<pData->dst_srv_type<<"]";
                    break;
                }
            }

            return;
        }
        default:
        {
            break;
        }
    }

    // 丢队列
    ModuleDataCenter::Instance()->PutProtocolData(prt);
}

// 套接字事件处理器
void CmsClientManager::Event(unsigned short seq, EM_NET_EVENT msg)
{
    switch (msg)
    {
        case ENE_CLOSE:
            LOG(ERROR)<<"连接关闭.";
            DelClient(seq);
            break;
        case ENE_HEART_TIMEOUT:
            LOG(ERROR)<<"心跳超时,关闭连接.";
            DelClient(seq);
            break;
        default:
            break;
    }
}

void CmsClientManager::Accept(SOCKET fd, struct sockaddr_in* sa)
{
    // 获取一个连接序号
    unsigned short seq = NetFrame::GetGloabSeq();

    PassiveTCPClient* pPassiveTCPClient = new(std::nothrow) PassiveTCPClient(
                seq, sa, SYS_CONFIG->get_module_config().cms_service_heartbeat_detection);
    do
    {
        if (nullptr == pPassiveTCPClient)
        {
            break;
        }

        if (!pPassiveTCPClient->StartWork(fd, this))
        {
            LOG(ERROR)<<"启动客户端失败.";
            break;
        }

        if (AddClient(seq, pPassiveTCPClient) != FUNC_SUCCESS)
        {
            break;
        }

        LOG(INFO)<<"收到CMS客户端连接:"<<inet_ntoa(sa->sin_addr)<<":"<<ntohs(sa->sin_port);

        return;

    }while(0);

    if (pPassiveTCPClient)
    {
        pPassiveTCPClient->StopWork();
        delete pPassiveTCPClient;
    }

    close(fd);
}
