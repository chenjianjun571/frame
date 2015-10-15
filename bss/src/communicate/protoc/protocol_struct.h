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

typedef struct stProtocolBase
{
    /// 消息ID
    jsbn::protoc::MSG command_id;
    /// 连接序号
    unsigned short sock_handle;

}TProtocolBase;

typedef struct stLogin:public TProtocolBase
{
    char user_name[15];
    char pass_word[15];

    stLogin() {
        clear();
    }

    void clear() {
        ::memset(user_name, 0x00, sizeof(user_name));
        ::memset(pass_word, 0x00, sizeof(pass_word));
    }
}TLogin;

typedef struct stLoginResp:public TProtocolBase
{
    int result;
    char desc[15];

    stLoginResp() {
        clear();
    }

    void clear() {
        ::memset(desc, 0x00, sizeof(desc));
    }
}TLoginResp;

typedef std::shared_ptr<TProtocolBase> sProtocolData_ptr;

#endif //__PROTOCOL_STRUCT_H_

