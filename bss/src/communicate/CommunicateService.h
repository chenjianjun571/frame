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
#ifndef __COMMUNICATE_SERVICE_H_
#define __COMMUNICATE_SERVICE_H_

#include "ModuleConstDef.h"
#include "./service/business/BusinessService.h"

class CommunicateService : public COperationObject
{
public:
    /// @brief 启动业务模块
    virtual bool start_operation();

    /// @brief 停止业务模块
    virtual bool stop_operation();

    virtual int dump(const char* first_param = nullptr, const char* second_param = nullptr);

private:
    // 业务服务器
    BusinessService _mBusinessService;
};

#endif
