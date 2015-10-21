///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           module_config_collection.h
/// @brief          配置信息解析
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月15日
/// @Description
/// @History
///************************************************************
#ifndef __MODULE_CONFIG_COLLECTION_H_
#define __MODULE_CONFIG_COLLECTION_H_

#include "module_const_def.h"

/// @brief 通信服务器配置信息
struct stModuleConfig
{
    // 服务器IP
    std::string host_ip;
    // 数据中心数据处理线程个数
    int data_proc_thread_num;

    // 业务服务器监听port
    int bss_service_listen_port;
    // 业务服务器心跳检测间隔时间
    int bss_service_heartbeat_detection;

    // rpc配置参数
    int rpc_listen_port;
    int rpc_proc_thread_num;

    // 数据库信息
    std::string db_host;
    int db_port;
    std::string db_name;
    std::string db_user;
    std::string db_password;
    int db_max_con_num;
};

/// @class CModuleConfigCollection 
/// @brief 用于管理各模块私有的配置信息
class CModuleConfigCollection : public CSystemConfigCollection
{
public:

    /// @brief 构造函数
	CModuleConfigCollection (void);

    /// @brief 析构函数
	virtual ~CModuleConfigCollection (void);

    /// @brief 获取类的唯一实例
    static CModuleConfigCollection* get_instance ();

	/// 状态打印
	virtual int dump(const char* first_param = NULL,
		const char* second_param = NULL);

    /// @brief 加载模块私有的配置文件信息
    /// @return FUNC_SUCCESS 加载成功，FUNC_FAILED加载失败，加载失败，启动将启动失败
    virtual int load_module_file_config (CIniFile &ini_filereader);

    // 获取模块自己的配置信息
    inline stModuleConfig& get_module_config ();
private:
    // 通信服务器配置信息
    stModuleConfig m_srv_config;
};

#define SYS_CONFIG (CModuleConfigCollection::get_instance ())

inline stModuleConfig& CModuleConfigCollection::get_module_config ()
{
    return m_srv_config;
}

#endif //__MODULE_CONFIG_COLLECTION_H_


