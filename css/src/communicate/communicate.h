///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
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
#include "./communicate/protoc/pb/protoc.pb.h"

// 协议数据智能指针
typedef std::shared_ptr<jsbn::protoc::NetProtocol> sNetProtocolDataPage_ptr;

// 地域信息
typedef struct stBssClientInfo
{
    // 城市ID
    jsbn::protoc::CityID city_id;

    stBssClientInfo()
    {
        city_id = jsbn::protoc::CityID::CID_INIT;
    }

}TBssClientInfo;

#endif
