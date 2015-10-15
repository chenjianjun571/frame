///************************************************************
/// @Copyright (C), 1993-2012, bafei  Information Technologies Co., Ltd.
/// @URL            
/// @file           src/frame/system_const_def.h
/// @brief          此文件中定义整个系统中公共使用的常量信息
/// @attention      
/// @Author         lijing
/// @Version        0.1
/// @Date           2012年12月21日 星期五 09时03分43秒
/// @Description    
/// @History        
///************************************************************

#if !defined (__SYSTEM_CONST_DEF_H_FILE__)
#define __SYSTEM_CONST_DEF_H_FILE__

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

const char* const gcv_product_name = "Global bafei";
const char* const gcv_product_version = "Version 1.0";

// 磁盘最小剩余空间（MB）
const unsigned int MIN_DISK_SPACE_SIZE= 500;

// 文件名路径最大长度
const unsigned int gcv_maxlen_filepath = 1024;
const unsigned int gcv_maxlen_error_info = 256;

// 函数返回值
#define    FUNC_SUCCESS  0
#define    FUNC_FAILED  -1

const unsigned int KILL_PROCESS = 1;

const char *const gcv_argv_options = ":c::w:l:kvhtdsu";
// 程序启动参数表
// -s(--startup)    启动进程（默认为守护进程方式）
// -u(--shutdown)   终止进程
// -c(--config)     修改配置文件
// -k(--console)    按控制台方式启动进程（无守护进程）
// -d(--display)    在控制台上显示日志信息
// -h(--help)       显示帮助信息
// -v(--version)    显示程序版本信息
// -t(--status)     察看程序当前运行状况
// -w(--watch)      根据类名查看程序内部相关数据及系统运行状态
// -l(--loglevel)   日志打印级别操作命令，根据该命令带的参数决定具体的操作类型
//                    c    设置日志打印级别为“致命错误”
//                    e    设置日志打印级别为“一般错误”及以上等级
//                    m    设置日志打印级别为“重要信息”及以上等级
//                    i    设置日志打印级别为“一般信息”及以上等级
//                    d    设置日志打印级别为“调试信息”及以上等级
//                    s    显示日志打印级别
// 执行程序时的参数信息存放的文件名, 此都是相对于程序所在目录的相对路径
const char * const gcv_paraminfo_filename = "data/Parameter.dat";
// 程序版本信息
const char * const gcv_versioninfo_filename = "data/version.dat";

#endif //__SYSTEM_CONST_DEF_H_FILE__

