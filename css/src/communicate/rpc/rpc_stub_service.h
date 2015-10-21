///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           rpc_stub_service.h
/// @brief          RPC服务器
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月19日
/// @Description
/// @History
///************************************************************
#ifndef __RPC_STUB_SERVICE_H_
#define __RPC_STUB_SERVICE_H_
#include "module_const_def.h"

#include <server/TNonblockingServer.h>
#include <concurrency/ThreadManager.h>

class RpcStubService:public jsbn::Runnable
{
public:
    RpcStubService();

    // 启动服务器
    int Start();

    // 停止服务器
    void Stop();

protected:
    virtual void Run(void*);

private:
    boost::shared_ptr<apache::thrift::server::TNonblockingServer> _service;
    boost::shared_ptr<apache::thrift::concurrency::ThreadManager> _thread_manager;
    jsbn::Thread _thread_main_rpc;
};

#endif
