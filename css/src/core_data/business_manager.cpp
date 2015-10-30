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
#include "./business/relay/proc_relay.h"

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
    ProcBase::Register(jsbn::protoc::CommandID::Register_Req, new ProcBssRegister());
    // 注册数据转发业务
    ProcBase::Register(jsbn::protoc::CommandID::Data_Relay, new ProcRelay());


//    {
//        try
//        {
//            mysqlpp::ScopedConnection scp = mysqlpp::ScopedConnection(*jsbn::DBOpInstance::Instance()->GetDBPool(), true);


//            //ScopedConnectionPtr scp = jsbn::DBOpInstance::Instance()->GetConnect();
//            if(nullptr == *scp)
//            {
//                LOG(ERROR)<<"获取数据库连接失败";
//                return false;
//            }

//            mysqlpp::Query query = scp->query();
//            query<<"select VIDEO_ID,NAME,REMARK,URL,IS_USED from jsbn_video";
//            query.store();
//            LOG(INFO)<<"Query:";//<<query.preview();

//            if (mysqlpp::StoreQueryResult res = query.store())
//            {
//                for (size_t i = 0; i < res.num_rows(); ++i)
//                {
//                    LOG(INFO)<<res[i];
//                }
//            }
//        }
//        catch(const mysqlpp::BadQuery& e)
//        {
//            LOG(ERROR)<<"检索失败:"<<e.what();
//        }
//        catch (const mysqlpp::Exception& er)
//        {
//            LOG(ERROR)<<"失败:"<<er.what();
//        }
//        catch (...)
//        {
//            LOG(ERROR)<<"未知错误";
//        }
//    }


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

BusinessManager::ProcDataRunnable::ProcDataRunnable()
{}

BusinessManager::ProcDataRunnable::~ProcDataRunnable()
{}

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
