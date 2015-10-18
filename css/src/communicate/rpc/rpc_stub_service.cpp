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

RpcStubService::RpcStubService()
{}

int RpcStubService::Start()
{
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
    boost::shared_ptr<RpcServiceHandler> handler(new RpcServiceHandler());
    boost::shared_ptr<TProcessor> processor(new RpcServiceProcessor(handler));

    boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(20);
    boost::shared_ptr<PosixThreadFactory> threadFactory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();

    _service =new TNonblockingServer(processor, protocolFactory, 6889, threadManager);

    _service->serve();

    return FUNC_SUCCESS;
}

void RpcStubService::Stop()
{
    _service->stop();
}
