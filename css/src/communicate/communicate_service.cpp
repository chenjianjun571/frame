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
#include "communicate_service.h"

bool CommunicateService::start_operation()
{
    if (_mBSSService.Start() != FUNC_SUCCESS)
    {
        LOG(ERROR)<<"业务服务器模块启动失败";
        return false;
    }

    if (_mRpcStubService.Start() != FUNC_SUCCESS)
    {
        LOG(ERROR)<<"RPC服务器模块启动失败";
        return false;
    }

    return true;
}

bool CommunicateService::stop_operation()
{
    _mBSSService.Stop();
    _mRpcStubService.Stop();
    return true;
}

int CommunicateService::dump(const char* first_param, const char* second_param)
{
    return 0;
}
