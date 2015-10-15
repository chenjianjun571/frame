///************************************************************
/// @Copyright (C), 2015-2030, bafei  Information Technologies Co., Ltd.
/// @URL
/// @file           business_control.cpp
/// @brief          应用程序中各子业务模块（观察者）的控制类文件（通知者）,业务的组织控制，
///                 通讯对象的产生，实现了对各业务子模块的全部启动、停止功能、配置变更通知。
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月07日
/// @Description
/// @History
///************************************************************
#include <glog/logging.h>
#include "./business_control.h"
#include "../system_public_def.h"
#include "../system_const_def.h"
#include "../operation_object.h"
#include "../console_resource.h"

CBusinessControl::CBusinessControl(void)
{
    m_thread_runing = false;
}

CBusinessControl::~CBusinessControl(void)
{
    m_thread_runing = false;
}

int CBusinessControl::start_system_control()
{
    // 初始化业务信息
    if (FUNC_SUCCESS != init_business_info ())
    {
        LOG(ERROR)<<IDS_INIT_MODULE_FAILED;
        return FUNC_FAILED;
    }

    // 创建各子业务对象
    if (!create_operate_object())
    {
        LOG(ERROR)<<IDS_CREATE_CHILD_MODULE_FAILED;
        return FUNC_FAILED;
    }

    //    启动各子业务对象
    if (!notify_operator_starting())
    {
        LOG(ERROR)<<IDS_START_CHILD_MODULE_FAILED;
        return FUNC_FAILED;
    }

    m_thread_runing = true;

    return FUNC_SUCCESS;
}

int CBusinessControl::stop_business_service ()
{
    // 停止之前清理
    cleanup_resource ();

    // 通知停止操作
    notify_operator_stopping ();

    return FUNC_SUCCESS;
}


void CBusinessControl::cleanup_resource ()
{
}

bool CBusinessControl::create_operate_object()
{
    return true;
}

int CBusinessControl::init_business_info ()
{
    return FUNC_SUCCESS;
}

bool CBusinessControl::notify_operator_starting()
{

    TOperationObjectList::iterator iter;
    if (m_operator_list.size() == 0)
    {
        //        return false;
    }

    iter = m_operator_list.begin();
    for (; iter != m_operator_list.end(); iter++)
    {
        if (!(*iter)->start_operation())
        {
            return false;
        }
    }

    return true;
}

bool CBusinessControl::notify_operator_stopping()
{
    TOperationObjectList::iterator iter;

    if (m_operator_list.size() == 0)
    {
        return false;
    }

    iter = m_operator_list.begin();

    for (; iter != m_operator_list.end(); iter++)
    {
        if ((*iter)->stop_operation())
        {
            return false;
        }
    }

    return true;
}


bool CBusinessControl::register_operator(COperationObject *obv)
{
    if (obv == NULL)
    {
        return false;
    }

    m_operator_list.push_back(obv);

    return true;
}

bool CBusinessControl::unregister_operator(COperationObject *obv)
{
    if (obv != NULL)
    {
        m_operator_list.remove(obv);

        return true;
    }

    return false;
}
