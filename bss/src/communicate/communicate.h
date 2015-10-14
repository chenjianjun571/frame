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

#include "module_const_def.h"
#include "../common/malloc_struct_factory.h"
#include "./communicate/protoc/pb/jsbn_bss.pb.h"
// 协议数据智能指针
typedef std::shared_ptr<jsbn::protoc::BSSNetProtocol> sNetProtocolDataPage_ptr;

#endif
