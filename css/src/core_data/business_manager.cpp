///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           business_manager.cpp
/// @brief          业务管理中心
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月23日
/// @Description
/// @History
///************************************************************
#include "business_manager.h"
#include "../module_config_collection.h"
#include "../module_data_center.h"
#include "./business/proc_base.h"
#include "./business/bss_register/proc_bss_register.h"

BusinessManager& BusinessManager::Instance()
{
    static BusinessManager instance;
    return instance;
}

BusinessManager::BusinessManager():_run_flg(false)
{}

bool BusinessManager::Start()
{
    // 注册业务服务器注册业务
    ProcBase::Register(jsbn::protoc::CommandID::Register_Req, new ProcBssRegister);

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

bool BusinessManager::Stop()
{
    _run_flg = false;

    for (std::vector<jsbn::Thread*>::iterator iter=_proc_thread.begin(); iter!=_proc_thread.end(); iter++)
    {
        delete *iter;
    }

    ProcBase::Destroy();

    return true;
}

BusinessManager::ProcDataRunnable::ProcDataRunnable() {}
BusinessManager::ProcDataRunnable::~ProcDataRunnable() {}

void BusinessManager::ProcDataRunnable::Run(void* arg)
{
    BusinessManager* pBusinessManager = (BusinessManager*)arg;
    sProtocolData_ptr prt;
    while (pBusinessManager->_run_flg)
    {
        prt = ModuleDataCenter::Instance()->GetProtocolData(1000);
        if (prt != nullptr)
        {
            ProcBase::Execute(prt);
        }
    }
}
