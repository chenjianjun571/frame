///************************************************************
/// @Copyright (C), 2015-2025, bafei  Information Technologies Co., Ltd.
/// @URL            
/// @file           console_resource.h
/// @brief          框架宏定义文件
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年04月29日
/// @Description
/// @History
///************************************************************
#ifndef __CONSOLE_RESOURCE_H_
#define __CONSOLE_RESOURCE_H_

#include "./I18N.h"


// 系统启动运行级提示信息
#define IDS_PROMPT_SYSTEM_STARTFAILED       _("系统启动失败。")
#define IDS_PROMPT_SYSTEM_STARTSUCCESS      _("系统启动成功。")
#define IDS_PROMPT_SYSTEM_SHUTDOWNSUCCESS   _("系统关闭成功。")
#define IDS_SYSTEM_RUNING                   _("系统正在运行。")
#define IDS_SYSTEM_FILE_BAD                 _("系统文件已损坏，请重新安装。")
#define IDS_GET_ABS_PATH_FAILED				_("获取进程运行绝对路径失败。")
#define IDS_NEW_MEMORY_FAILED               _("内存分配失败。")
#define IDS_OPENFILE_FAILED_STRERROR        _("打开文件(%s)失败: %s。")


// 启动参数类
#define IDS_PROMPT_SYSTEM_HELPINFO          _("用法:%s  [-s] [-d] [-k] \n\t  [-u]|[-c]|[-h]|[-v]|[-t]\n\t  [-w class-name]|[-l parameter]")
#define IDS_PROMPT_HELPINFO_ERROR           _("获取帮助信息失败")
#define IDS_PROMPT_VERSION_ERROR            _("获取版本信息失败")
#define IDS_PROGRAM_ALREADY_STARTED         _("已启动了一个程序实例。")
#define IDS_KILL_PROGRAM_INSTANCE           _("当前进程将被终止，请稍候...")
#define IDS_NON_SYSTEM_INSTANCE             _("没有正在运行的程序实例。")
#define IDS_BACKSTORE_FILE_CREATE_FAILED	_("系统创建进程共享内存后备存储文件失败。")
#define IDS_SHARE_MEMORY_CREATE_FAILED      _("系统创建共享内存池失败。")
#define IDS_SHARE_MEMORY_INIT_FAILED        _("系统初始化共享内存池失败。")
#define IDS_DISK_SPACE_INVALIED				_("程序运行磁盘空间不足 %d MB，仅仅剩余 %d MB。")
#define IDS_DISK_SPACE_GET_FAILED			_("获取磁盘空间失败，错误码：%d。")
#define IDS_CREATE_CHILD_MODULE_FAILED		_("创建并挂载进程内子业务模块失败。")
#define IDS_START_CHILD_MODULE_FAILED		_("启动进程内子业务模块失败。")
#define IDS_NO_PROCESS_RUNNING_COMMAND_INVALID	        _("进程未启动，当前命令 -%c 无效。 \n")
#define IDS_INVALID_COMMAND_OR_PARAM_ERROR		        _("无效的选项命令或者参数错误，请参考正确的用法。")

#define IDS_INIT_MODULE_FAILED		_("模块初始化失败。")


// 系统日志工厂相关字符串
#define  IDS_CURRENT_LOGGER_LEVEL				_("当前日志打印级别信息：等级ID = %d, 等级描述 = %s。\n")
#define	 IDS_SET_LOG_LEVEL_SUCCESS				_("设置新日志打印级别 -%c(%s) 成功。\n")
#define	 IDS_SET_LOG_LEVEL_FAILED				_("设置新日志打印级别 -%c(%s) 失败。\n")
#define  IDS_LOG_LEVEL_FATAL					_("致命错误")
#define  IDS_LOG_LEVEL_ERROR					_("一般错误")
#define  IDS_LOG_LEVEL_IMPORTANCE				_("重要信息")
#define  IDS_LOG_LEVEL_INFO						_("一般信息")
#define  IDS_LOG_LEVEL_DEBUG					_("调试日志")
#define  IDS_INVALID_LOG_OPER_PARAM				_("无效的日志级别操作参数。")
#define  IDS_LOGGER_LEVEL_UPDATE				_("检测到日志打印级别设置变更，更新为：%s 。")
#define  IDS_READ_LOG_BACKUP_SIZE_FALIED		_("读取日志备份目录容量阈值失败，原因：%s 。")
#define  IDS_LOG_BACKUP_SIZE_OVERLOAD			_("日志备份文件夹容量超阈值：阈值 = %dM，当前大小 = %dM。")
#define  IDS_LOG_WCOMMAND_ERROR                 _("未知的命令参数。\n")


#define  IDS_OBJ_MONITOR_CLASS_NAME_ERROE		_("类名[ %s ]无效或者输入不合法，请检查！\n")
#define  IDS_LOG_FILE_BACKUP_THREAD_START		_("系统日志自动备份线程启动。")
#define  IDS_LOG_FILE_BACKUP_THREAD_EXIT		_("系统日志自动备份线程退出。")


// 日志对象创建失败提示日志
#define IDS_CONSOLE_LOG_CREATE_ERROR                _("创建控制台日志失败。")
#define IDS_MEMORY_LOG_CREATE_ERROR                 _("创建内存日志失败。")
#define IDS_FILE_LOG_CREATE_ERROR                   _("创建文件日志失败。")
//#define IDS_DATABASE_LOG_CREATE_ERROR               _("创建数据库日志失败。")
#define IDS_FILE_LOG_CREATE_OTHER_ERROR             _("创建文件日志过程其它错误。")
#define IDS_CREATE_LOGDIR_FAILED                    _("创建日志目录(%s)失败。")
#define IDS_OPEN_FILE_FAILED                        _("打开文件(%s)失败, errno=%d。")
#define IDS_WRITE_LOG_HEADER_FAILED                 _("写日志文件头信息失败。")

#endif//CONSOLE_INFOX_RESOURCE
