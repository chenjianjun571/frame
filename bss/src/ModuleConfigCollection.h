///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           ModuleConfigCollection.h
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

#include "ModuleConstDef.h"

/// @brief 通信服务器配置信息
struct stModuleConfig
{
    // 转发监听IP
    std::string relay_ip;
    // 转发监听port 例如:"1111,2222,1122"
    std::string relay_ports;

    /// 跟信令服务器的链接心跳
    std::string signal_ip;
    int signal_port;
    int signal_srv_heartbeat_time;

    stModuleConfig()
    {
        relay_ip.clear();
        signal_ip.clear();
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


