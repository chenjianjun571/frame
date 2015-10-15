///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           css_client.h
/// @brief          中心服务器客户端
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月10日
/// @Description
/// @History
///************************************************************
#ifndef __CSS_CLIENT_H_
#define __CSS_CLIENT_H_

#include "module_const_def.h"

class CSSClient :
        public sigslot::has_slots<>,
        public jsbn::TCPClientSignal,
        public jsbn::Runnable
{
public:
    CSSClient();

    ~CSSClient();

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

protected:
    virtual void Run(void* arg);

private:
    jsbn::ActiveTCPClient* _pActiveTCPClient;
    // 运行标志
    volatile bool _is_run_flg;
    // 连接检测线程
    jsbn::Thread _connect_thread;
};

#endif
