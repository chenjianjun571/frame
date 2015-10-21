///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           rpc_stub_service.cpp
/// @brief          RPC服务器
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月19日
/// @Description
/// @History
///************************************************************
#include "rpc_stub_service.h"
#include "module_config_collection.h"
#include "RpcServiceHandler.h"
#include "./server/RpcService.h"

#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>
#include <server/TNonblockingServer.h>

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace apache::thrift::concurrency;

RpcStubService::RpcStubService():_thread_manager(nullptr),_service(nullptr)
{}

int RpcStubService::Start()
{
    if (_service)
    {
        return FUNC_SUCCESS;
    }

    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    boost::shared_ptr<RpcServiceHandler> handler(new RpcServiceHandler());
    boost::shared_ptr<TProcessor> processor(new RpcServiceProcessor(handler));
    boost::shared_ptr<PosixThreadFactory> threadFactory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());

    do
    {
        _thread_manager = ThreadManager::newSimpleThreadManager(SYS_CONFIG->get_module_config().rpc_proc_thread_num);
        if (_thread_manager == nullptr) {
            break;
        }

        _service = boost::shared_ptr<TNonblockingServer>(
                    new TNonblockingServer(processor,
                                           protocolFactory,
                                           SYS_CONFIG->get_module_config().rpc_listen_port,
                                           _thread_manager));
        if (_service == nullptr) {
            break;
        }

        _thread_manager->threadFactory(threadFactory);
        _thread_manager->start();

        if (!_thread_main_rpc.Start(this))
        {
            break;
        }

        return FUNC_SUCCESS;

    }while(0);

    _thread_main_rpc.Stop();

    if (_thread_manager)
    {
        _thread_manager->stop();
        _thread_manager=nullptr;
    }

    if (_service)
    {
        _service->stop();
        _service=nullptr;
    }

    return FUNC_FAILED;
}

void RpcStubService::Stop()
{
    if (_thread_manager)
    {
        _thread_manager->stop();
        _thread_manager=nullptr;
    }

    if (_service)
    {
        _service->stop();
        _service=nullptr;
    }
}

void RpcStubService::Run(void*)
{
    _service->serve();
}
