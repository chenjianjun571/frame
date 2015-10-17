///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           communicate.h
/// @brief          通信模块头文件
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月21日
/// @Description
/// @History
///************************************************************
#ifndef __COMMUNICATE_H_
#define __COMMUNICATE_H_

#include "module_const_def.h"
#include "../common/malloc_struct_factory.h"
#include "./communicate/protoc/pb/jsbn_css.pb.h"

// BSS与CSS的协议数据智能指针
typedef std::shared_ptr<jsbn::protoc::BCNetProtocol> sBCNetProtocolDataPage_ptr;

// SMS与CSS的协议数据智能指针
typedef std::shared_ptr<jsbn::protoc::SCNetProtocol> sSCNetProtocolDataPage_ptr;

// 地域信息
typedef struct stBssClientInfo
{
    // 城市ID
    EM_CITY_ID city_id;

    stBssClientInfo()
    {
        city_id = ECID_INIT;
    }

}TBssClientInfo;

#endif
