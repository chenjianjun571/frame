///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           ModuleBusinessControl.h
/// @brief          业务控制器
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月15日
/// @Description
/// @History
///************************************************************
#ifndef __MODULE_BUSINESS_CONTROL_H_
#define __MODULE_BUSINESS_CONTROL_H_

#include "ModuleConstDef.h"

/// @class CModuleBusinessControl 
/// @brief 模块业务控制类，用于创建并挂接各子业务
class CModuleBusinessControl : public CBusinessControl
{
public:
    static CModuleBusinessControl* get_instance ();

protected:
    /// @brief 初始化业务信息
    /// @detail 在启动业务之前将调用此函数
    /// @return FUNC_SUCCESS 初始化成功, FUNC_FAILED初始化失败
    virtual int init_business_info();

    /// @brief 关闭应用程序时，释放此类中所有资源，包括停止各子业务。 
    /// @detail 在退出系统之间将启用此函数
    virtual void cleanup_resource();

    /// @brief 创建各子业务对象，并且把新创建的字业务对象加入到业务对象
    ///        列表中。实现业务的挂载功能。
    /// @return  false 失败, 此时系统将启动失败；true 成功
    virtual bool create_operate_object();

};

#define MODULE_BUSINESS_CONTROLER (CModuleBusinessControl::get_instance ())

#endif //__MODULE_BUSINESS_CONTROL_H_

