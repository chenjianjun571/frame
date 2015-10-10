///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           ISSService.h
/// @brief          接入服务器监听服务器
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月10日
/// @Description
/// @History
///************************************************************
#ifndef __ISS_SERVICE_H_
#define __ISS_SERVICE_H_

#include "ModuleConstDef.h"

class ISSService :
        public sigslot::has_slots<>,
        public jsbn::TCPClientSignal,
        public jsbn::TCPServerSignal
{
public:
    ISSService();

    ~ISSService();

    // 启动服务器
    int Start();

    // 停止服务器
    void Stop();

    int SendData(const sSendDataPage_ptr& pSend);

public:
    // 数据接收
    void RecvData(unsigned short, const unsigned char*, PacketLength);

    // 套接字事件处理器
    void Event(unsigned short fd, EM_NET_EVENT msg);

    // 客户端连接触发器
    void Accept(SOCKET fd, struct sockaddr_in* sa);

protected:
    int AddClient(unsigned short, jsbn::PassiveTCPClient*);
    void DelClient(unsigned short);

private:
    jsbn::ServerWorker* _pServerWorker;
    std::map<unsigned short, jsbn::PassiveTCPClient*> _map_clients;
    jsbn::RWLock* _client_mutex;
};

#endif
