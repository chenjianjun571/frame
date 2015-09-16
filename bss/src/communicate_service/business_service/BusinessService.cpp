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

using namespace jsbn;

BusinessService::BusinessService():_pServerWorker(nullptr),_client_mutex(RWLock::Create())
{}

BusinessService::~BusinessService()
{
    delete _client_mutex;
}

int BusinessService::Start() {

    //TODO: "192.168.1.4",8888 从配置文件读取
    _pServerWorker = new(std::nothrow) ServerWorker("192.168.1.4",8888);
    if (nullptr == _pServerWorker) {
        return FUNC_FAILED;
    }

    SignalAccept.connect(this, &BusinessService::Accept);
    SignalAcceptError.connect(this, &BusinessService::Event);
    SignalRecvData.connect(this, &BusinessService::RecvData);
    SignalEvent.connect(this, &BusinessService::Event);

    if (!_pServerWorker->StartWork(this)) {
        LOG(ERROR)<<"服务器监听启动失败";
        return FUNC_FAILED;
    }

    return FUNC_SUCCESS;
}

void BusinessService::Stop() {

    if (nullptr == _pServerWorker) {
        return;
    }

    _pServerWorker->StopWork();

    WriteLockScoped wls(&_client_mutex);
    std::map<SOCKET, PassiveTCPClient*>::iterator it = _map_clients.begin();
    while (it != _map_clients.end()) {
        it->second->StopWork();
        delete it->second;
        _map_clients.erase(it++);
    }

}

int BusinessService::SendData(SOCKET fd, void* data, size_t len) {

    ReadLockScoped rls(&_client_mutex);
    std::map<SOCKET, PassiveTCPClient*>::iterator it = _map_clients.find(fd);
    if (it != _map_clients.end()) {
        return it->second->SendData(data, len);
    }

    return FUNC_FAILED;
}

int BusinessService::AddClient(SOCKET fd, jsbn::PassiveTCPClient* p_client) {

    WriteLockScoped wls(&_client_mutex);
    std::map<SOCKET, PassiveTCPClient*>::iterator it = _map_clients.find(fd);
    if (it != _map_clients.end()) {
        return FUNC_FAILED;
    }

    _map_clients[fd] = p_client;

    return FUNC_SUCCESS;
}

void BusinessService::DelClient(SOCKET fd) {

    WriteLockScoped wls(&_client_mutex);
    std::map<SOCKET, PassiveTCPClient*>::iterator it = _map_clients.find(fd);
    if (it != _map_clients.end()) {
        it->second->StopWork();
        delete it->second;
        _map_clients.erase(it);
    }
}

void BusinessService::RecvData(sRecvDataPage_ptr& data) {
    // 丢队列，让其他线程处理
}

// 套接字事件处理器
void BusinessService::Event(SOCKET fd, EM_NET_EVENT msg) {

    switch (msg) {
    case ENE_CLOSE:
        LOG(ERROR)<<"连接关闭";
        DelClient(fd);
        break;
    case ENE_ACCEPT_ERROR:
        LOG(ERROR)<<"监听失败";
        Stop();
        break;
    default:
        break;
    }

}

void BusinessService::Accept(SOCKET fd, struct sockaddr_in* sa) {

    LOG(ERROR)<<"收到客户端连接.";

    // TODO 15是心跳时间，需要读取配置文件
    PassiveTCPClient* pPassiveTCPClient = new PassiveTCPClient(fd, sa, 15);
    if (!pPassiveTCPClient->StartWork(this)) {
        LOG(ERROR)<<"启动客户端失败";
        delete pPassiveTCPClient;
        return;
    }

    if (AddClient(fd, pPassiveTCPClient) != FUNC_SUCCESS) {
        pPassiveTCPClient->StopWork();
        delete pPassiveTCPClient;
    }
}
