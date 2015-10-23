///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           proc_bss_register.h
/// @brief          处理BSS注册
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月21日
/// @Description
/// @History
///************************************************************
#include "../proc_base.h"

class ProcBssRegister:public ProcBase
{
public:
    ProcBssRegister();

    virtual ~ProcBssRegister();

    //具体处理方法，由派生类实现.
    virtual void Process(const sProtocolData_ptr& pMsg);
};
