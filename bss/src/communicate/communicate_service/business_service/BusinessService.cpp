///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           BusinessService.cpp
/// @brief          业务服务器
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#include "BusinessService.h"
#include "ModuleConfigCollection.h"
#include "../../communicate_protoc/ProtocolProcManager.h"
#include "../../../ModuleDataCenter.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace jsbn;
using namespace jsbn::protoc;

BusinessService::BusinessService():_pServerWorker(nullptr),_client_mutex(RWLock::Create())
{}

BusinessService::~BusinessService()
{
    delete _client_mutex;
}

int BusinessService::Start()
{

    _pServerWorker = new(std::nothrow) ServerWorker(SYS_CONFIG->get_module_config().bus_listen_ip,
                                                    SYS_CONFIG->get_module_config().bus_listen_port);
    if (nullptr == _pServerWorker)
    {
        return FUNC_FAILED;
    }

    SignalAccept.connect(this, &BusinessService::Accept);
    SignalAcceptError.connect(this, &BusinessService::Event);
    SignalRecvData.connect(this, &BusinessService::RecvData);
    SignalEvent.connect(this, &BusinessService::Event);

    if (!_pServerWorker->StartWork(this))
    {
        LOG(ERROR)<<"服务器监听启动失败";
        return FUNC_FAILED;
    }

    return FUNC_SUCCESS;
}

void BusinessService::Stop()
{
    if (nullptr == _pServerWorker)
    {
        return;
    }

    _pServerWorker->StopWork();

    WriteLockScoped wls(*_client_mutex);
    std::map<SOCKET, PassiveTCPClient*>::iterator it = _map_clients.begin();
    while (it != _map_clients.end())
    {
        it->second->StopWork();
        delete it->second;
        _map_clients.erase(it++);
    }

}

int BusinessService::SendData(const sSendDataPage_ptr& pSend)
{
    ReadLockScoped rls(*_client_mutex);
    std::map<SOCKET, PassiveTCPClient*>::iterator it = _map_clients.find(pSend->sock_handle);
    if (it != _map_clients.end())
    {
        return it->second->SendData(pSend);
    }

    return FUNC_FAILED;
}

int BusinessService::AddClient(SOCKET fd, jsbn::PassiveTCPClient* p_client)
{
    WriteLockScoped wls(*_client_mutex);
    std::map<SOCKET, PassiveTCPClient*>::iterator it = _map_clients.find(fd);
    if (it != _map_clients.end()) {
        return FUNC_FAILED;
    }

    _map_clients[fd] = p_client;

    return FUNC_SUCCESS;
}

void BusinessService::DelClient(SOCKET fd)
{
    WriteLockScoped wls(*_client_mutex);
    std::map<SOCKET, PassiveTCPClient*>::iterator it = _map_clients.find(fd);
    if (it != _map_clients.end()) {
        it->second->StopWork();
        delete it->second;
        _map_clients.erase(it);
    }
}

void BusinessService::RecvData(SOCKET fd, const unsigned char* buf, PacketLength len)
{
    // 解析数据协议
    sNetProtocolDataPage_ptr prt = ProtocolProcManager::ParseProtocol(buf, len);
    if (nullptr == prt) {
        LOG(ERROR)<<"协议解析失败，关闭连接.";
        DelClient(fd);
        return;
    }

    if (prt->type() == Heart_Beat) {
        LOG(INFO)<<"收到心跳.";
        // 心跳协议
        sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
        pSend->Copy(buf, len);

        SendData(pSend);
        return;
    }

    // 丢队列
    ModuleDataCenter::Instance()->PutRecvData(prt);
}

// 套接字事件处理器
void BusinessService::Event(SOCKET fd, EM_NET_EVENT msg)
{

    switch (msg)
    {
    case ENE_CLOSE:
        LOG(ERROR)<<"连接关闭.";
        DelClient(fd);
        break;
    case ENE_ACCEPT_ERROR:
        LOG(ERROR)<<"监听失败.";
        Stop();
        break;
    case ENE_HEART_TIMEOUT:
        LOG(ERROR)<<"心跳超时,关闭连接.";
        DelClient(fd);
        break;
    default:
        break;
    }

}

void BusinessService::Accept(SOCKET fd, struct sockaddr_in* sa)
{
    LOG(INFO)<<"收到客户端连接:"<<::inet_ntoa(sa->sin_addr)<<":"<<::ntohs(sa->sin_port);

    PassiveTCPClient* pPassiveTCPClient = new PassiveTCPClient(fd, sa, SYS_CONFIG->get_module_config().bus_heartbeat_detection);
    if (!pPassiveTCPClient->StartWork(this))
    {
        LOG(ERROR)<<"启动客户端失败.";
        delete pPassiveTCPClient;
        return;
    }

    if (AddClient(fd, pPassiveTCPClient) != FUNC_SUCCESS)
    {
        pPassiveTCPClient->StopWork();
        delete pPassiveTCPClient;
    }
}