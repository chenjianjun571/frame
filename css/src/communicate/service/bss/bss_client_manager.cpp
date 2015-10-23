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

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace jsbn;
using namespace jsbn::protoc;

static BssClientManager& BssClientManager::Instance()
{
    static BssClientManager instance;
    return instance;
}

BssClientManager::BssClientManager():_pServerWorker(nullptr),_client_mutex(RWLock::Create())
{}

BssClientManager::~BssClientManager()
{
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
    if (nullptr == _pServerWorker)
    {
        return;
    }

    _pServerWorker->StopWork();

    WriteLockScoped wls(*_client_mutex);
    std::map<unsigned short, BssTcpClient*>::iterator it = _map_clients.begin();
    while (it != _map_clients.end())
    {
        it->second->StopWork();
        delete it->second;
        _map_clients.erase(it++);
    }

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
    std::map<unsigned short, BssTcpClient*>::iterator it = _map_clients.find(seq);
    if (it != _map_clients.end())
    {
        it->second->StopWork();
        delete it->second;
        _map_clients.erase(it);
    }
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

void BssClientManager::SetBssClinentInfo(unsigned short seq, TBssClientInfo& info)
{
    ReadLockScoped rls(*_client_mutex);
    std::map<unsigned short, BssTcpClient*>::iterator it = _map_clients.find(seq);
    if (it != _map_clients.end())
    {
        it->second->SetBssClinentInfo(info);
    }
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
//            TBssClientInfo info;
//            info.city_id = static_cast<EM_CITY_ID>(((TRegisterRequest*)prt.get())->city_id);
//            {
//                ReadLockScoped rls(*_client_mutex);
//                std::map<unsigned short, BssTcpClient*>::iterator it = _map_clients.find(seq);
//                if (it != _map_clients.end())
//                {
//                    it->second->SetBssClinentInfo(info);
//                }
//            }

//            {
//                // 测试一个注册应答
//                std::string response;
//                jsbn::protoc::NetProtocol pc;
//                pc.set_commandid(jsbn::protoc::CommandID::Register_Rsp);
//                pc.mutable_registerrsp()->set_result(0);
//                pc.mutable_registerrsp()->set_error_description("注册成功");
//                pc.SerializeToString(&response);
                
//                sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
//                pSend->sock_handle = prt->sock_handle;
//                pSend->Copy(response.c_str(), response.length());
//                SendData(pSend);
//            }

            break;
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
