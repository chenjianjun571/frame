///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           business_control.h
/// @brief          业务控制中心
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月23日
/// @Description
/// @History
///************************************************************
#ifndef __BUSINESS_CONTROL_H_
#define __BUSINESS_CONTROL_H_

#include "module_const_def.h"
#include <vector>

class BusinessControl
{
public:
    BusinessControl();

    /// @brief 启动业务模块
    virtual bool Start();

    /// @brief 停止业务模块
    virtual bool Stop();

protected:
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
