///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           bss_service.h
/// @brief          业务服务器(地域服务器)监听服务器
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月13日
/// @Description
/// @History
///************************************************************
#ifndef __BSS_SERVICE_H_
#define __BSS_SERVICE_H_

#include "module_const_def.h"
#include "bss_tcp_client.h"

class BSSService :
        public sigslot::has_slots<>,
        public jsbn::TCPClientSignal,
        public jsbn::TCPServerSignal
{
public:
    BSSService();

    ~BSSService();

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
    int AddClient(unsigned short, BssTcpClient*);
    void DelClient(unsigned short);
    bool CheckClient(unsigned short);

private:
    jsbn::ServerWorker* _pServerWorker;
    std::map<unsigned short, BssTcpClient*> _map_clients;
    jsbn::RWLock* _client_mutex;
};

#endif
