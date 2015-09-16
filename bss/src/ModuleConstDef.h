///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           ModuleConstDef.h
/// @brief          模块静态定义类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月15日
/// @Description
/// @History
///************************************************************
#ifndef __MODULE_CONST_DEF_H_
#define __MODULE_CONST_DEF_H_

#include <ace/Basic_Types.h>
#include <ace/Thread_Mutex.h>
#include <ace/OS.h>
#include <ace/Date_Time.h>
#include <ace/Condition_T.h>
#include <ace/Reactor.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Auto_Ptr.h>
#include <ace/Task.h>
#include <ace/Event_Handler.h>
#include <ace/Log_Msg.h>
#include <ace/Reactor.h>
#include <ace/Signal.h>
#include <ace/streams.h>
#include <ace/Thread_Manager.h>
#include <ace/TP_Reactor.h>
#include <ace/RW_Thread_Mutex.h>

#include <map>
#include <string>
#include <vector>
#include <list>

/// 包含框架库头文件
#include <ExInclude.h>

#include "./communicate_protoc/protoc/jsbn_bss.pb.h"

// 模块配置文件名
#define MODULE_CONFIG_FILENAME "config/bss.ini"
// 模块配置文件名 gcv_module_config_filename的值就是MODULE_CONFIG_FILENAME 
extern const char* const gcv_module_config_filename;
// 模块名称
const char* const gcv_module_name = "bss";
// 模块可执行文件名
const char* const gcv_program_name = "bss";

// 协议数据智能指针
typedef std::shared_ptr<jsbn::protoc::NetProtocol> sNetProtocolDataPage_ptr;

#endif //__MODULE_CONST_DEF_H_

