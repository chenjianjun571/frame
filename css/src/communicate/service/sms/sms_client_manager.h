///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           sms_client_manager.h
/// @brief          系统管理服务器管理端
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年11月7日
/// @Description
/// @History
///************************************************************
#ifndef __SMS_CLIENT_MANAGER_H_
#define __SMS_CLIENT_MANAGER_H_

#include "module_const_def.h"
#include "../communicate.h"

class SmsClientManager : public sigslot::has_slots<>,
        public jsbn::TCPClientSignal,
        public jsbn::TCPServerSignal
{
public:
    static SmsClientManager& Instance();

    // 启动服务器
    int Start();

    // 停止服务器
    void Stop();

    // 发送数据
    int SendData(const sSendDataPage_ptr& pSend);

protected:
    SmsClientManager();

    ~SmsClientManager();

    // 数据接收
    void RecvData(unsigned short, const unsigned char*, PacketLength);

    // 套接字事件处理器
    void Event(unsigned short fd, EM_NET_EVENT msg);

    // 客户端连接触发器
    void Accept(SOCKET fd, struct sockaddr_in* sa);

    int AddClient(unsigned short seq, jsbn::PassiveTCPClient* p_client);
    
    void DelClient(unsigned short seq);

private:
    jsbn::ServerWorker* _pServerWorker;
    jsbn::PassiveTCPClient* _sms_client;
    jsbn::RWLock* _client_mutex;
};

#endif