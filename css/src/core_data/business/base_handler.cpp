///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           proc_base.cpp
/// @brief          业务处理基类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月21日
/// @Description
/// @History
///************************************************************
#include "module_const_def.h"

class ProcBase
{
protected:
    ProcBase();
    virtual ~ProcBase();

    virtual void ProcData();
};
