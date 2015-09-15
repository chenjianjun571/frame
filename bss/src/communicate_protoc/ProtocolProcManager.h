///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           ProtocolProcManager.h
/// @brief          协议处理管理类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#ifndef __PROTOCOL_PROC_MANAGER_H_
#define __PROTOCOL_PROC_MANAGER_H_

#include "ModuleConstDef.h"

class ProtocolProcManager
{
public:
    ProtocolProcManager(){};
    // 解析通信协议
    static sNetProtocolDataPage_ptr ParseProtocol(jsbn::sRecvDataPage_ptr& data);
};

#endif //__PROTOCOL_PROC_MANAGER_H_

