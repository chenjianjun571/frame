///************************************************************
/// @Copyright (C), 1993-2022, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           protocol_proc_manager.h
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

#include "module_const_def.h"
#include "protocol_struct.h"
#include "../communicate.h"

class ProtocolProcManager
{
public:
    static sProtocolData_ptr ParseProtocol(const unsigned char*, PacketLength);
};

#endif //__PROTOCOL_PROC_MANAGER_H_

