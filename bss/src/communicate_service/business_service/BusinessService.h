///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           BusinessService.h
/// @brief          业务服务器
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#ifndef __BUSINESS_SERVICE_H_
#define __BUSINESS_SERVICE_H_

#include "ModuleConstDef.h"

class BusinessService :
        public sigslot::has_slots<>,
        public jsbn::TCPClientSignal,
        public jsbn::TCPServerSignal
{
public:
    BusinessService();

    ~BusinessService();

    // 启动服务器
    int Start();

    // 停止服务器
    void Stop();

    int SendData(SOCKET fd, void* data, size_t len);

public:
    // 数据接收
    void RecvData(SOCKET, const unsigned char*, jsbn::PacketLength);

    // 套接字事件处理器
    void Event(SOCKET fd, jsbn::EM_NET_EVENT msg);

    // 客户端连接触发器
    void Accept(SOCKET fd, struct sockaddr_in* sa);

protected:
    int AddClient(SOCKET, jsbn::PassiveTCPClient*);
    void DelClient(SOCKET);

private:
    jsbn::ServerWorker* _pServerWorker;
    std::map<SOCKET, jsbn::PassiveTCPClient*> _map_clients;
    jsbn::RWLock* _client_mutex;
};

#endif
