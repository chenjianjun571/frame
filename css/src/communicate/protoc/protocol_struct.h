///************************************************************
/// @Copyright (C), 1993-2022, jsbn  Information Technologies Co., Ltd.
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

// 通信协议基类
typedef struct stProtocolBase
{
    /// 消息ID
    jsbn::protoc::CommandID command_id;
    /// 连接序号
    unsigned short sock_handle;

}TProtocolBase;
typedef std::shared_ptr<TProtocolBase> sProtocolData_ptr;

typedef struct stRegisterRequest : public TProtocolBase
{
    // 城市ID
    unsigned int city_id;

}TRegisterRequest;

typedef struct stRegisterResponse : public TProtocolBase
{
    // 城市ID
    unsigned int result;
    // 描述
    std::string error_description;

}TRegisterResponse;

#endif //__PROTOCOL_STRUCT_H_

