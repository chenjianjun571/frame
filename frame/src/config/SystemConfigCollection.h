///************************************************************
/// @Copyright (C), 1993-2012, bafei  Information Technologies Co., Ltd.
/// @URL            
/// @file           src/frame/config/SystemConfigCollection.h
/// @brief          系统配置文件类, 用于管理各模块公有的配置信息，以及License信息
/// @attention      
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2012年12月25日 星期二 17时49分11秒
/// @Description    
/// @History        
///************************************************************

#ifndef _SYS_INFO_COLLECTION_H
#define _SYS_INFO_COLLECTION_H

#include "../DumpObject.h"


class CIniFile;

/// @class SSysFileConfig
/// @brief 系统公共配置文件信息
struct SSystemFileConfig
{
};

/// @class SDatabaseConfig
/// @brief 数据库配置信息
struct SDatabaseConfig
{
    /// 数据库实例名
    std::string db_name;
    /// 数据库地址
    std::string db_addr;
    /// 用户名
    std::string user_name;
    /// 密码
    std::string user_password;
    /// 连接数
    int connect_num;
};

/// @class CSystemConfigCollection
/// @brief 系统业务配置数据的的存储类。保存各种配置数据，并提供访问接口。
/// @todo 各应用程序在此类中添加自己的配置数据存储。
class CSystemConfigCollection : CDumpObject
{
public:
    /// @brief 构造函数
	CSystemConfigCollection(void);

    /// @brief 析构函数
	virtual ~CSystemConfigCollection(void);

	/// 状态打印
	virtual int dump(const char* first_param = NULL,
                                            const char* second_param = NULL);

    /// @brief 加载各模块公有的配置文件信息
    /// @param[in] filename 公有配置文件的路径名，路径为绝对路径
    /// @return FUNC_SUCCESS 加载成功，FUNC_FAILED加载失败，加载失败，启动将启动失败
    int load_system_file_config (const char *filename, ACE_Process_Mutex &file_mutex);

    /// @brief 加载License配置信息
    /// @param[in] filename license配置文件的路径名，路径为绝对路径
    /// @return FUNC_SUCCESS 加载成功，FUNC_FAILED加载失败，加载失败，启动将启动失败
    int load_system_license_config (const char *filename);

    /// @brief 加载模块私有有的配置文件信息
    /// @param[in] filename 模块私有配置文件的路径名，路径为绝对路径
    /// @return FUNC_SUCCESS 加载成功，FUNC_FAILED加载失败，加载失败，启动将启动失败
    int load_module_file_config (const char *filename, ACE_Process_Mutex &file_mutex);

    /// @brief 加载模块私有的配置文件信息
    /// @return FUNC_SUCCESS 加载成功，FUNC_FAILED加载失败，加载失败，启动将启动失败
    virtual int load_module_file_config (CIniFile &ini_filereader) = 0;

    /// @brief 读取数据配置信息
    /// @detail 读取的信息将保存在get_database_config()函数返回的对象中
    /// @return FUNC_SUCCESS读取成功，FUNC_FAILED读取失败
    virtual int load_database_config (CIniFile &ini_filereader);

    inline SDatabaseConfig& get_database_config ();
    inline SSystemFileConfig& get_sysfile_config ();
protected:
    /// @brief 读取配置文件中的字符串配置项
    /// @param[in] section 配置项所在的节
    /// @param[in] item配置项在节中的名字
    /// @param[out] str_val 读取成功，此为读取的配置项
    /// @return FUNC_SUCCESS读取成功，FUNC_FAILED读取失败
    int load_string_item (CIniFile &ini_reader, const char *section, const char *item, std::string &str_val);

    /// @brief 读取整数类配置项
    /// @param[in] section 配置项所在的节
    /// @param[in] item配置项在节中的名字
    /// @param[out] ret_val 读取成功，此为读取的配置项
    /// @param[in] rand_beg 此配置项的合法取值的开始值
    /// @param[in] rand_end 此配置项的合法取值的结束值
    /// @return FUNC_SUCCESS读取成功，FUNC_FAILED读取失败
    int load_int_item (CIniFile &ini_reader, const char *section, const char *item, int &ret_val, 
            int rang_beg, int rang_end);

private:
    /// 系统公有配置信息
    SSystemFileConfig m_sysfile_config;
    /// 各模块的数据库配置信息
    SDatabaseConfig m_database_config;
};

inline SDatabaseConfig& CSystemConfigCollection::get_database_config ()
{
    return m_database_config;
}
inline SSystemFileConfig& CSystemConfigCollection::get_sysfile_config ()
{
    return m_sysfile_config;
}

#endif


