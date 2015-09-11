/// *****************************************************************
/// @file    OperationObject.h
/// @warning    Copyright (C), 1993-2010, bafei  Information Technologies Co., Ltd.
///
/// @brief        此文件定义了一个观察者抽象基类。
///
/// @attention  应用程序中各个子业务、功能块继承于它，根据自己特征实现自己的业务控制，
///                可以作为各个子业务、各子业务内部功能块的启停控制类。
/// @todo   
///
/// @version    1.0
///
/// @author        mone
///
/// @date        2009/01/09 
///
/// @note        <author>    <date>            <note>  \n   
///
/// *****************************************************************
#ifndef _OPERATOR_OBJECT_H
#define _OPERATOR_OBJECT_H

#include "./DumpObject.h"

/// @class COperationObject
/// @brief 此类是观察者模式中的观察者抽象基类，具有两个纯虚函数和一个虚函数：
///        @c start_operation启动自己；@c stop_operation 停止自己；
///        @c notify_change函数用于通知者类进行配置变更通知。子类需要全部
///        实现此类中的两个纯虚函数，并且返回有效的返回值。
///        同时它继承了@c CDumpObject接口，它的子类也具有被动态状态查看的功能，其
///        子类需要实现@c CDumpObject的接口，同时把自己添加到@c CObjectMonitorFactory
///        对象监控工厂中。
class COperationObject : public CDumpObject
{
public:
    /// @brief 默认构造函数
    COperationObject(void)  
    {
        this->m_class_name = "COperationObject";
    };

    /// @brief 析构函数
    virtual ~COperationObject(void) {};

public:

    /// @brief        启动各个子业务模块。纯虚函数。
    /// @return        false ,失败；true , 成功
    virtual bool start_operation() = 0;

    /// @brief        停止各个子业务模块。纯虚函数。
    /// @return        false ,失败；true , 成功
    virtual bool stop_operation() = 0;

    /// 
    /// @author mone
    /// @date    2010/05/06
    /// @brief    通知各个子业务模块的内部配置变更。虚函数。
    /// @details 子业务内部如果有配置变更，则需要实现此函数，如果内部没有配置变更，
    ///             则可不用覆盖（override）此虚函数。
    ///    @param[in]    notify_req    配置变更通知请求包
    /// @return        false ,失败；true , 成功
#if 0
    virtual bool notify_change(ConfigNotify_package* notify_req)
    {
        return true;
    }
#endif
};

#endif

