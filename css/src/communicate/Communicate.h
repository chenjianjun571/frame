///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           Communicate.h
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

#include "ModuleConstDef.h"
#include "../common/MallocStructFactory.h"

#include "./communicate/protoc/pb/jsbn_css.pb.h"
// 协议数据智能指针
typedef std::shared_ptr<jsbn::protoc::CSSNetProtocol> sNetProtocolDataPage_ptr;

#endif
