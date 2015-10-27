///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           proc_relay.h
/// @brief          处理转发
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月27日
/// @Description
/// @History
///************************************************************
#include "../proc_base.h"

class ProcRelay:public ProcBase
{
public:
    ProcRelay();

    virtual ~ProcRelay();

    //具体处理方法，由派生类实现.
    virtual void Process(const sProtocolData_ptr& pMsg);
};
