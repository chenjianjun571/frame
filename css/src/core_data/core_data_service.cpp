///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           core_data_service.cpp
/// @brief          核心数据服务模块
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月21日
/// @Description
/// @History
///************************************************************
#include "core_data_service.h"
#include "business_manager.h"

CoreDataService::CoreDataService()
{}

bool CoreDataService::start_operation()
{
    if (!BusinessManager::Instance().Start())
    {
        LOG(ERROR)<<"业务控制中心启动失败";
        return false;
    }

    return true;
}

bool CoreDataService::stop_operation()
{
    BusinessManager::Instance().Stop();
    return true;
}

int CoreDataService::dump(const char* first_param, const char* second_param)
{
    return 0;
}
