///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           ProtocolStruct.h
/// @brief          协议结构类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月26日
/// @Description
/// @History
///************************************************************
#ifndef __PROTOCOL_STRUCT_H_
#define __PROTOCOL_STRUCT_H_

#include "ModuleConstDef.h"
#include "../Communicate.h"

// BSS与CSS的通信协议基类
typedef struct stBCProtocolBase
{
    /// 消息ID
    jsbn::protoc::BC_MSG command_id;
    /// 连接序号
    unsigned short sock_handle;

}TBCProtocolBase;
typedef std::shared_ptr<TBCProtocolBase> sBCProtocolData_ptr;

// SMS与CSS的通信协议基类
typedef struct stSCProtocolBase
{
    /// 消息ID
    jsbn::protoc::SC_MSG command_id;
    /// 连接序号
    unsigned short sock_handle;

}TSCProtocolBase;
typedef std::shared_ptr<TSCProtocolBase> sSCProtocolData_ptr;

#endif //__PROTOCOL_STRUCT_H_

