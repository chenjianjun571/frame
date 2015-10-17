///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           protocol_struct.h
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

#include "module_const_def.h"
#include "../communicate.h"

// BSS与CSS的通信协议基类
typedef struct stBCProtocolBase
{
    /// 消息ID
    jsbn::protoc::bc::CommandID command_id;
    /// 连接序号
    unsigned short sock_handle;

}TBCProtocolBase;
typedef std::shared_ptr<TBCProtocolBase> sBCProtocolData_ptr;

typedef struct stBCRegisterRequest
{
    // 城市ID
    unsigned int city_id;

}TBCRegisterRequest;

typedef struct stBCRegisterResponse
{
    // 城市ID
    unsigned int result;
    // 描述
    std::string error_description;

}TBCRegisterResponse;

#endif //__PROTOCOL_STRUCT_H_

