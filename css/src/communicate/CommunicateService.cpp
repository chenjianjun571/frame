///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           CommunicateService.h
/// @brief          通信服务模块
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#include "CommunicateService.h"

bool CommunicateService::start_operation()
{
    if (_mISSService.Start() != FUNC_SUCCESS)
    {
        LOG(ERROR)<<"接入服务器模块启动失败";
        return false;
    }

    if (_mTSSService.Start() != FUNC_SUCCESS)
    {
        LOG(ERROR)<<"终端服务器模块启动失败";
        return false;
    }

    if (_mCSSClient.Start() != FUNC_SUCCESS)
    {
        LOG(ERROR)<<"终端服务器模块启动失败";
        return false;
    }

    return true;
}

bool CommunicateService::stop_operation()
{
    _mISSService.Stop();
    _mTSSService.Stop();
    _mCSSClient.Stop();
    return true;
}

int CommunicateService::dump(const char* first_param, const char* second_param)
{
    return 0;
}
