///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
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

bool CoreDataService::start_operation()
{
    _proc_thread.clear();

    jsbn::Thread* pThread = new jsbn::Thread();

    return true;
}

bool CoreDataService::stop_operation()
{
    return true;
}

int CoreDataService::dump(const char* first_param, const char* second_param)
{
    return 0;
}
