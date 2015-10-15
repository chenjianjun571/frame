///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
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
    // 接入服务器监听IP
    std::string iss_service_listen_ip;
    // 接入服务器监听port
    int iss_service_listen_port;
    // 接入服务器心跳检测间隔时间
    int iss_service_heartbeat_detection;

    // 终端服务器监听IP
    std::string tss_service_listen_ip;
    // 终端服务器监听port
    int tss_service_listen_port;
    // 终端服务器心跳检测间隔时间
    int tss_service_heartbeat_detection;

    // 中心服务器IP
    std::string css_client_listen_ip;
    // 中心服务器监听port
    int css_client_listen_port;
    // 心跳检测间隔时间
    int css_client_heartbeat_detection;

    stModuleConfig()
    {
        iss_service_listen_ip.clear();
        iss_service_heartbeat_detection = 15;
        tss_service_listen_ip.clear();
        tss_service_heartbeat_detection = 15;
        css_client_listen_ip.clear();
        css_client_heartbeat_detection = 15;
    }
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
    stModuleConfig m_net_srv_config;
};

#define SYS_CONFIG (CModuleConfigCollection::get_instance ())

inline stModuleConfig& CModuleConfigCollection::get_module_config ()
{
    return m_net_srv_config;
}

#endif //__MODULE_CONFIG_COLLECTION_H_


