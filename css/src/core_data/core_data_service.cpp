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
#include "../module_config_collection.h"
#include "../module_data_center.h"
#include "./business/proc_base.h"

CoreDataService::CoreDataService():_run_flg(false)
{}

bool CoreDataService::start_operation()
{
    _proc_thread.clear();
    _run_flg = false;

    for(int i = 0; i < CModuleConfigCollection::get_instance()->get_module_config().data_proc_thread_num; ++i)
    {
        jsbn::Thread* pThread = new(std::nothrow) jsbn::Thread();
        if (!pThread->Start(&_runnable))
        {
            LOG(ERROR)<<"处理线程创建失败";
            return false;
        }

        _proc_thread.push_back(pThread);
    }

    return true;
}

bool CoreDataService::stop_operation()
{
    _run_flg = false;

    for (std::vector<jsbn::Thread*>::iterator iter=_proc_thread.begin(); iter!=_proc_thread.end(); iter++)
    {
        delete *iter;
    }

    return true;
}

int CoreDataService::dump(const char* first_param, const char* second_param)
{
    return 0;
}

CoreDataService::ProcDataRunnable::ProcDataRunnable() {}
CoreDataService::ProcDataRunnable::~ProcDataRunnable() {}

void CoreDataService::ProcDataRunnable::Run(void* arg)
{

    CoreDataService* pCoreDataService = (CoreDataService*)arg;
    sProtocolData_ptr prt;
    while (pCoreDataService->_run_flg)
    {
        prt = ModuleDataCenter::Instance()->GetProtocolData(1000);
        if (prt != nullptr)
        {
            ProcBase::Execute(prt);
        }
    }
}
