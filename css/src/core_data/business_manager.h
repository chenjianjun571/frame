///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           business_manager.h
/// @brief          业务管理中心
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月23日
/// @Description
/// @History
///************************************************************
#ifndef __BUSINESS_MANAGER_H_
#define __BUSINESS_MANAGER_H_

#include "module_const_def.h"
#include <vector>

class BusinessManager
{
public:
    static BusinessManager& Instance();

    /// @brief 启动业务模块
    virtual bool Start();

    /// @brief 停止业务模块
    virtual bool Stop();

protected:
    BusinessManager();

    class ProcDataRunnable:public jsbn::Runnable
    {
    public:
        ProcDataRunnable();
        ~ProcDataRunnable();
    protected:
        virtual void Run(void*);
    };

    friend class ProcDataRunnable;

private:
    // 数据处理线程组
    std::vector<jsbn::Thread*> _proc_thread;
    volatile bool _run_flg;
    ProcDataRunnable _runnable;
};

#endif
