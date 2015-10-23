///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           communicate_service.h
/// @brief          通信服务模块
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#ifndef __COMMUNICATE_SERVICE_H_
#define __COMMUNICATE_SERVICE_H_

#include "module_const_def.h"
#include "./rpc/rpc_stub_service.h"

class CommunicateService : public COperationObject
{
public:
    /// @brief 启动业务模块
    virtual bool start_operation();

    /// @brief 停止业务模块
    virtual bool stop_operation();

    virtual int dump(const char* first_param = nullptr, const char* second_param = nullptr);

private:

    // RPC服务器
    RpcStubService _mRpcStubService;
};

#endif
