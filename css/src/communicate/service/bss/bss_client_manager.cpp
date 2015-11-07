///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           bss_client_manager.cpp
/// @brief          业务服务器管理端
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月13日
/// @Description
/// @History
///************************************************************
#include "bss_client_manager.h"
#include "module_config_collection.h"
#include "../../protoc/protocol_proc_manager.h"
#include "../../../module_data_center.h"
#include "../cms/cms_client_manager.h"
#include "../sms/sms_client_manager.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace jsbn;
using namespace jsbn::protoc;

BssClientManager& BssClientManager::Instance()
{
    static BssClientManager instance;
    return instance;
}

BssClientManager::BssClientManager():_pServerWorker(nullptr),_client_mutex(RWLock::Create())
{}

BssClientManager::~BssClientManager()
{
    Stop();
    delete _client_mutex;
}

int BssClientManager::Start()
{
    _pServerWorker = new(std::nothrow) ServerWorker(SYS_CONFIG->get_module_config().host_ip,
                                                    SYS_CONFIG->get_module_config().bss_service_listen_port);
    if (nullptr == _pServerWorker)
    {
        return FUNC_FAILED;
    }

    SignalAccept.connect(this, &BssClientManager::Accept);
    SignalRecvData.connect(this, &BssClientManager::RecvData);
    SignalEvent.connect(this, &BssClientManager::Event);

    if (!_pServerWorker->StartWork(this))
    {
        LOG(ERROR)<<"服务器监听启动失败";
        return FUNC_FAILED;
    }

    return FUNC_SUCCESS;
}

void BssClientManager::Stop()
{
    if (_pServerWorker)
    {
        _pServerWorker->StopWork();
        delete _pServerWorker;
        _pServerWorker = nullptr;
    }

    WriteLockScoped wls(*_client_mutex);
    for(auto& it:_map_clients)
    {
        it.second->StopWork();
        delete it.second;
    }
    _map_clients.clear();

    for(auto& it:_map_city_clients)
    {
        it.second->StopWork();
        delete it.second;
    }
    _map_city_clients.clear();
}

int BssClientManager::SendData(const sSendDataPage_ptr& pSend)
{
    ReadLockScoped rls(*_client_mutex);
    std::map<unsigned short, BssTcpClient*>::iterator it = _map_clients.find(pSend->sock_handle);
    if (it != _map_clients.end())
    {
        return it->second->SendData(pSend->send_buf, pSend->send_len);
    }

    return FUNC_FAILED;
}

int BssClientManager::SendData(jsbn::protoc::CityID city_id, const sSendDataPage_ptr& pSend)
{
    ReadLockScoped rls(*_client_mutex);
    std::map<jsbn::protoc::CityID, BssTcpClient*>::iterator it =  _map_city_clients.find(city_id);
    if (it != _map_city_clients.end())
    {
        return it->second->SendData(pSend->send_buf, pSend->send_len);
    }

    return FUNC_FAILED;
}

void BssClientManager::SetBssClinentInfo(unsigned short seq, const TBssClientInfo& info)
{
    ReadLockScoped rls(*_client_mutex);
    std::map<unsigned short, BssTcpClient*>::iterator it = _map_clients.find(seq);
    if (it != _map_clients.end())
    {
        it->second->SetBssClinentInfo(info);
        _map_city_clients[info.city_id] = it->second;
    }
}

int BssClientManager::AddClient(unsigned short seq, BssTcpClient* p_client)
{
    WriteLockScoped wls(*_client_mutex);
    std::map<unsigned short, BssTcpClient*>::iterator it = _map_clients.find(seq);
    if (it != _map_clients.end())
    {
        return FUNC_FAILED;
    }

    _map_clients[seq] = p_client;

    return FUNC_SUCCESS;
}

void BssClientManager::DelClient(unsigned short seq)
{
    WriteLockScoped wls(*_client_mutex);
    jsbn::protoc::CityID city_id = jsbn::protoc::CityID::CID_INIT;
    std::map<unsigned short, BssTcpClient*>::iterator it = _map_clients.find(seq);
    if (it != _map_clients.end())
    {
        it->second->StopWork();
        city_id = it->second->GetBssClinentInfo().city_id;
        delete it->second;
        _map_clients.erase(it);
    }

    _map_city_clients.erase(city_id);
}

bool BssClientManager::CheckClient(unsigned short seq)
{
    ReadLockScoped rls(*_client_mutex);
    std::map<unsigned short, BssTcpClient*>::iterator it = _map_clients.find(seq);
    if (it != _map_clients.end())
    {
        return it->second->Isvalid();
    }

    return false;
}

void BssClientManager::RecvData(unsigned short seq, const unsigned char* buf, PacketLength len)
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
        case jsbn::protoc::CommandID::Register_Req:// 注册请求
        {
            TBssClientInfo info;
            info.city_id = ((TRegisterRequest*)prt.get())->city_id;

            LOG(INFO)<<"收到注册请求，注册城市id["<<info.city_id<<"]";
            SetBssClinentInfo(prt->sock_handle, info);

            // 注册应答
            std::string response;
            jsbn::protoc::NetProtocol pc;
            pc.set_commandid(jsbn::protoc::CommandID::Register_Rsp);
            pc.mutable_registerrsp()->set_result(0);
            pc.mutable_registerrsp()->set_error_description("注册成功");
            pc.SerializeToString(&response);

            sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
            pSend->sock_handle = prt->sock_handle;
            pSend->Copy(response.c_str(), response.length());

            SendData(pSend);

            return;
        }
        case jsbn::protoc::CommandID::Data_Relay_Req:// 数据转发请求
        {
            // 检测该客户端是否已经注册，未注册的客户端不提供服务
            if (!CheckClient(seq))
            {
                LOG(ERROR)<<"非法客户端试图发送数据，断开连接.";
                DelClient(seq);
                return;
            }

            TDataRelayReq* pData = (TDataRelayReq*)prt.get();
            sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
            pSend->sock_handle = prt->sock_handle;
            pSend->Copy(buf, len);

            switch (pData->dst_srv_type)
            {
                case jsbn::protoc::ServiceTpye::ST_CMS:
                {
                    if(CmsClientManager::Instance().SendData(pSend) != FUNC_SUCCESS)
                    {
                        LOG(ERROR)<<"转发数据失败,CMS客户端不在线";

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
                        LOG(INFO)<<"转发数据成功,目的服务器:CMS.";
                    }

                    break;
                }
                case jsbn::protoc::ServiceTpye::ST_SMS:
                {
                    if(SmsClientManager::Instance().SendData(pSend) != FUNC_SUCCESS)
                    {
                        LOG(ERROR)<<"转发数据失败,SMS客户端不在线";

                        std::string response;
                        jsbn::protoc::NetProtocol pc;

                        // 转发应答
                        pc.set_commandid(jsbn::protoc::CommandID::Data_Relay_Rsp);
                        pc.mutable_datarelayrsp()->set_srcsrvtype(pData->src_srv_type);
                        pc.mutable_datarelayrsp()->set_dstsrvtype(pData->dst_srv_type);
                        pc.mutable_datarelayrsp()->set_dstcityid(pData->dst_city_id);
                        pc.mutable_datarelayrsp()->set_result(-1);
                        pc.mutable_datarelayrsp()->set_error_description("转发数据失败,SMS客户端不在线");

                        pc.SerializeToString(&response);

                        sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
                        pSend->sock_handle = prt->sock_handle;
                        pSend->Copy(response.c_str(), response.length());

                        SendData(pSend);
                    }
                    else
                    {
                        LOG(INFO)<<"转发数据成功,目的服务器:SMS.";
                    }

                    break;
                }
                default:
                {
                    LOG(ERROR)<<"BSS无效的转发服务器类型.["<<pData->dst_srv_type<<"]";
                    break;
                }
            }

            return;
        }
        default:
        {
            // 检测该客户端是否已经注册，未注册的客户端不提供服务
            if (!CheckClient(seq))
            {
                LOG(ERROR)<<"非法客户端试图发送数据，断开连接.";
                DelClient(seq);
                return;
            }

            break;
        }
    }

    // 丢队列
    ModuleDataCenter::Instance()->PutProtocolData(prt);
}

// 套接字事件处理器
void BssClientManager::Event(unsigned short seq, EM_NET_EVENT msg)
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

void BssClientManager::Accept(SOCKET fd, struct sockaddr_in* sa)
{
    // 获取一个连接序号
    unsigned short seq = NetFrame::GetGloabSeq();

    BssTcpClient* pBssTcpClient = new(std::nothrow) BssTcpClient(
                seq, sa, SYS_CONFIG->get_module_config().bss_service_heartbeat_detection);
    do
    {
        if (nullptr == pBssTcpClient)
        {
            break;
        }

        if (!pBssTcpClient->StartWork(fd, this))
        {
            LOG(ERROR)<<"启动客户端失败.";
            break;
        }

        if (AddClient(seq, pBssTcpClient) != FUNC_SUCCESS)
        {
            LOG(ERROR)<<"客户端添加失败.";
            break;
        }

        pBssTcpClient->SetIp(inet_ntoa(sa->sin_addr));
        LOG(INFO)<<"收到客户端连接:"<<inet_ntoa(sa->sin_addr)<<":"<<ntohs(sa->sin_port);

        return;

    }while(0);

    if (pBssTcpClient)
    {
        pBssTcpClient->StopWork();
        delete pBssTcpClient;
    }

    close(fd);
}
