///************************************************************
/// @Copyright (C), 2015-2030, bafei  Information Technologies Co., Ltd.
/// @URL
/// @file           business_control.h
/// @brief          应用程序中各子业务模块（观察者）的控制类文件（通知者）,业务的组织控制，
///                 通讯对象的产生，实现了对各业务子模块的全部启动、停止功能、配置变更通知。
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月07日
/// @Description
/// @History
///************************************************************
#ifndef _BUSINISS_CONTROL_H_
#define _BUSINISS_CONTROL_H_

#include <list>

// 类型向前申明
class COperationObject; 

///     定义观察者队列
typedef std::list<COperationObject *> TOperationObjectList;


/// @class CBusinessControl
/// @brief 此类是观察者模式中的通知者类，也即是应用程序中各子业务的总控制类，
///        负责通知注册到它中的各子业务的启动和停止。其观察者为
///        @c COperationObject类的子类。目前主要实现了对各业务子模块的全部启动、
///        停止以及内部配置变更通知功能。
/// @todo  可以继续完善其他业务控制。
class CBusinessControl
{
public:
    ///    默认构造函数
    CBusinessControl(void);

    ///    默认析构函数 
    virtual ~CBusinessControl(void);

public: 

    /// @brief 启动系统控制器。在此函数中创建并启动各种子业务。
    /// @return FUNC_SUCCESS 启动成功, FUNC_FAILED启动失败，此时进程应该退出。
    int start_system_control();

    /// @brief 停止系统各子功能块
    int stop_business_service ();

private:

    /// @brief  通知各个子业务模块的控制类（观察者）启动自己所有的业务。
    /// @return false ,失败；true , 成功
    bool notify_operator_starting();

    /// @brief  通知各个子业务模块的控制类（观察者）停止自己所有的业务。
    /// @return false ,失败；true , 成功
    bool notify_operator_stopping();

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
    virtual bool create_operate_object() = 0;

    /// @brief 登记某个子业务模块的控制类（观察者）。
    /// @param[in] obv 具体的观察者（@c COperationObject子类对象）
    /// @return false ,失败；true , 成功
    bool  register_operator(COperationObject *obv);

    /// @brief 注销某个子业务模块的控制类（观察者）。
    /// @param[in] obv 具体的观察者（@c COperationObject子类对象）
    /// @return false ,失败；true , 成功
    bool  unregister_operator(COperationObject *obv);

private:
    /// 观察者队列（子业务队列）
    TOperationObjectList m_operator_list;

    bool m_thread_runing;
};

#endif

