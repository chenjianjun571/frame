///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           core_data_service.h
/// @brief          核心数据服务模块
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月21日
/// @Description
/// @History
///************************************************************
#ifndef __CORE_DATA_SERVICE_H_
#define __CORE_DATA_SERVICE_H_

#include "module_const_def.h"
#include "business_control.h"
#include <vector>

class CoreDataService : public COperationObject
{
public:
    CoreDataService();

    /// @brief 启动业务模块
    virtual bool start_operation();

    /// @brief 停止业务模块
    virtual bool stop_operation();

    virtual int dump(const char* first_param = nullptr,
                     const char* second_param = nullptr);

private:
    // 数据处理线程组
    BusinessControl _business_control;
};

#endif
