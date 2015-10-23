///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           business_control.cpp
/// @brief          业务控制中心
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月23日
/// @Description
/// @History
///************************************************************
#include "business_control.h"
#include "../module_config_collection.h"
#include "../module_data_center.h"
#include "./business/proc_base.h"

BusinessControl::BusinessControl():_run_flg(false)
{}

bool BusinessControl::Start()
{
    // 注册业务

    // 启动业务处理线程
    _proc_thread.clear();
    _run_flg = true;
    for(int i = 0; i < CModuleConfigCollection::get_instance()->get_module_config().data_proc_thread_num; ++i)
    {
        jsbn::Thread* pThread = new(std::nothrow) jsbn::Thread();
        if (!pThread->Start(&_runnable, this))
        {
            LOG(ERROR)<<"处理线程创建失败";
            return false;
        }

        _proc_thread.push_back(pThread);
    }

    return true;
}

bool BusinessControl::Stop()
{
    _run_flg = false;

    for (std::vector<jsbn::Thread*>::iterator iter=_proc_thread.begin(); iter!=_proc_thread.end(); iter++)
    {
        delete *iter;
    }

    return true;
}

BusinessControl::ProcDataRunnable::ProcDataRunnable() {}
BusinessControl::ProcDataRunnable::~ProcDataRunnable() {}

void BusinessControl::ProcDataRunnable::Run(void* arg)
{
    BusinessControl* pBusinessControl = (BusinessControl*)arg;
    sProtocolData_ptr prt;
    while (pBusinessControl->_run_flg)
    {
        prt = ModuleDataCenter::Instance()->GetProtocolData(1000);
        if (prt != nullptr)
        {
            ProcBase::Execute(prt);
        }
    }
}
