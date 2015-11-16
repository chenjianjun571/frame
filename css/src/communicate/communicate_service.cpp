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
#include "./service/bss/bss_client_manager.h"
#include "./service/cms/cms_client_manager.h"
#include "./service/sms/sms_client_manager.h"

bool CommunicateService::start_operation()
{
    if (BssClientManager::Instance().Start() != FUNC_SUCCESS)
    {
        LOG(ERROR)<<"BSS服务器模块启动失败";
        return false;
    }
    LOG(INFO)<<"BSS服务器模块启动成功";

    if (CmsClientManager::Instance().Start() != FUNC_SUCCESS)
    {
        LOG(ERROR)<<"CMS服务器模块启动失败";
        return false;
    }
    LOG(INFO)<<"CMS服务器模块启动成功";

    if (SmsClientManager::Instance().Start() != FUNC_SUCCESS)
    {
        LOG(ERROR)<<"SMS服务器模块启动失败";
        return false;
    }
    LOG(INFO)<<"SMS服务器模块启动成功";

    if (_mRpcStubService.Start() != FUNC_SUCCESS)
    {
        LOG(ERROR)<<"RPC模块启动失败";
        return false;
    }
    LOG(INFO)<<"RPC模块启动成功";

    return true;
}

bool CommunicateService::stop_operation()
{
    BssClientManager::Instance().Stop();
    CmsClientManager::Instance().Stop();
    SmsClientManager::Instance().Stop();
    _mRpcStubService.Stop();
    return true;
}

int CommunicateService::dump(const char* first_param, const char* second_param)
{
    return 0;
}
