///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           bss_client_manager.h
/// @brief          业务服务器管理端
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月13日
/// @Description
/// @History
///************************************************************
#ifndef __BSS_CLIENT_MANAGER_H_
#define __BSS_CLIENT_MANAGER_H_

#include "module_const_def.h"
#include "../communicate.h"
#include "bss_tcp_client.h"

class BssClientManager : public sigslot::has_slots<>,
        public jsbn::TCPClientSignal,
        public jsbn::TCPServerSignal
{
public:
    static BssClientManager& Instance();

    // 启动服务器
    int Start();

    // 停止服务器
    void Stop();

    // 发送数据
    int SendData(const sSendDataPage_ptr& pSend);

    // 发送数据
    int SendData(jsbn::protoc::CityID, const sSendDataPage_ptr&);

    // BSS注册以后设置客户端信息
    void SetBssClinentInfo(unsigned short seq, const TBssClientInfo& info);

protected:
    BssClientManager();

    ~BssClientManager();

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
    jsbn::RWLock* _client_mutex;
    std::map<unsigned short, BssTcpClient*> _map_clients;
    std::map<jsbn::protoc::CityID, BssTcpClient*> _map_city_clients;
};

#endif
