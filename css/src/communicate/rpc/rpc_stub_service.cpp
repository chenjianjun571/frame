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
#include "./server/RpcServiceHandler.h"

#include <concurrency/ThreadManager.h>
#include <concurrency/PosixThreadFactory.h>
#include "./server/RpcService.h"
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
        _thread_manager = ThreadManager::newSimpleThreadManager(20);
        if (_thread_manager == nullptr) {
            break;
        }

        _service = new TNonblockingServer(processor, protocolFactory, 6889, _thread_manager);
        if (_service == nullptr) {
            break;
        }

        threadManager->threadFactory(threadFactory);
        threadManager->start();
        _service->serve();

        return FUNC_SUCCESS;

    }while(0);

    if (_thread_manager)
    {
        _thread_manager->stop();
        delete _thread_manager;
        _thread_manager = nullptr;
    }

    if (_service)
    {
        _service->stop();
        delete _service;
        _service = nullptr;
    }

    return FUNC_FAILED;
}

void RpcStubService::Stop()
{
    if (_thread_manager)
    {
        _thread_manager->stop();
        delete _thread_manager;
        _thread_manager = nullptr;
    }

    if (_service)
    {
        _service->stop();
        delete _service;
        _service = nullptr;
    }
}
