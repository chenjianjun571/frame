///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           TSSService.cpp
/// @brief          业务服务器
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#include "TSSService.h"
#include "ModuleConfigCollection.h"
#include "../../protoc/ProtocolProcManager.h"
#include "../../../ModuleDataCenter.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace jsbn;
using namespace jsbn::protoc;

TSSService::TSSService():_pServerWorker(nullptr),_client_mutex(RWLock::Create())
{}

TSSService::~TSSService()
{
    delete _client_mutex;
}

int TSSService::Start()
{
    _pServerWorker = new(std::nothrow) ServerWorker(SYS_CONFIG->get_module_config().tss_service_listen_ip,
                                                    SYS_CONFIG->get_module_config().tss_service_listen_port);
    if (nullptr == _pServerWorker)
    {
        return FUNC_FAILED;
    }

    SignalAccept.connect(this, &TSSService::Accept);
    SignalRecvData.connect(this, &TSSService::RecvData);
    SignalEvent.connect(this, &TSSService::Event);

    if (!_pServerWorker->StartWork(this))
    {
        LOG(ERROR)<<"服务器监听启动失败";
        return FUNC_FAILED;
    }

    return FUNC_SUCCESS;
}

void TSSService::Stop()
{
    if (nullptr == _pServerWorker)
    {
        return;
    }

    _pServerWorker->StopWork();

    WriteLockScoped wls(*_client_mutex);
    std::map<unsigned short, PassiveTCPClient*>::iterator it = _map_clients.begin();
    while (it != _map_clients.end())
    {
        it->second->StopWork();
        delete it->second;
        _map_clients.erase(it++);
    }

}

int TSSService::SendData(const sSendDataPage_ptr& pSend)
{
    ReadLockScoped rls(*_client_mutex);
    std::map<unsigned short, PassiveTCPClient*>::iterator it = _map_clients.find(pSend->sock_handle);
    if (it != _map_clients.end())
    {
        return it->second->SendData(pSend);
    }

    return FUNC_FAILED;
}

int TSSService::AddClient(unsigned short seq, jsbn::PassiveTCPClient* p_client)
{
    WriteLockScoped wls(*_client_mutex);
    std::map<unsigned short, PassiveTCPClient*>::iterator it = _map_clients.find(seq);
    if (it != _map_clients.end())
    {
        return FUNC_FAILED;
    }

    _map_clients[seq] = p_client;

    return FUNC_SUCCESS;
}

void TSSService::DelClient(unsigned short seq)
{
    WriteLockScoped wls(*_client_mutex);
    std::map<unsigned short, PassiveTCPClient*>::iterator it = _map_clients.find(seq);
    if (it != _map_clients.end())
    {
        it->second->StopWork();
        delete it->second;
        _map_clients.erase(it);
    }
}

void TSSService::RecvData(unsigned short seq, const unsigned char* buf, PacketLength len)
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

    // 判断是否心跳
    if (prt->command_id == Heart_Beat)
    {
        sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
        pSend->sock_handle = prt->sock_handle;
        pSend->Copy(buf, len);
        SendData(pSend);
        return;
    }

    // 丢队列
    ModuleDataCenter::Instance()->PutRecvData(prt);
}

// 套接字事件处理器
void TSSService::Event(unsigned short seq, EM_NET_EVENT msg)
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

void TSSService::Accept(SOCKET fd, struct sockaddr_in* sa)
{
    // 获取一个连接序号
    unsigned short seq = NetFrame::GetGloabSeq();

    PassiveTCPClient* pPassiveTCPClient = new(std::nothrow) PassiveTCPClient(
                seq, sa, SYS_CONFIG->get_module_config().tss_service_heartbeat_detection);
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
            LOG(ERROR)<<"客户端添加失败.";
            break;
        }

        LOG(INFO)<<"收到客户端连接:"<<inet_ntoa(sa->sin_addr)<<":"<<ntohs(sa->sin_port);

        return;

    }while(0);

    if (pPassiveTCPClient)
    {
        pPassiveTCPClient->StopWork();
        delete pPassiveTCPClient;
    }

    close(fd);
}
