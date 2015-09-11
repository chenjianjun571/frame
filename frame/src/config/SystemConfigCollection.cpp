/// ***************************************************************
/// @file        SystemConfigCollection.cpp
///
/// @warning    Copyright (C), 1993-2010, bafei  Information Technologies Co., Ltd.
///
/// @brief        系统级以及各子业务模块的配置信息集合类。此类用各种数据
///                结构存储系统、各子业务的配置信息，并提供相关的函数来访问
///                这些数据。
///
/// @attention  它是一个全局单体，可以在模块内任何地方使用。
///
/// @todo  
///
/// @version    1.0
///
/// @author        chenjianjun
///
/// @date        2009/02/05 
///
/// @note     <author>    <date>            <note>   \n      
///       
/// ***************************************************************
#include "./SystemConfigCollection.h"
#include "../ObjectMonitorFactory.h"
#include "./IniFile.h"
#include "../SystemConstDef.h"


CSystemConfigCollection::CSystemConfigCollection(void)
{

    this->m_class_name = "CSystemConfigCollection";

    OBJ_MONITOR->attach_dump_object(this);
}

CSystemConfigCollection::~CSystemConfigCollection(void)
{
    OBJ_MONITOR->dettach_dump_object(this);
}


int CSystemConfigCollection::dump(const char* first_param /* = NULL */, 
                                   const char* second_param /* = NULL */)
{
    return 0;
}

int CSystemConfigCollection::load_system_file_config (const char *filename, ACE_Process_Mutex &file_mutex)
{
    ACE_Guard<ACE_Process_Mutex> guard(file_mutex);

    return FUNC_SUCCESS;
}


int CSystemConfigCollection::load_system_license_config (const char *filename)
{
    return FUNC_SUCCESS;
}


int CSystemConfigCollection::load_module_file_config (const char *filename, ACE_Process_Mutex &file_mutex)
{

    CIniFile config_reader;

    ACE_Guard<ACE_Process_Mutex> guard(file_mutex);

    // 打开配置文件
    if (config_reader.read_open (filename) < 0)
    {
        printf ("打开配置文件(%s)失败.\n", filename);

        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_database_config (config_reader))
    {
        return FUNC_FAILED;
    }

    return load_module_file_config (config_reader);
}

int CSystemConfigCollection::load_module_file_config (CIniFile &ini_filereader)
{
    return FUNC_SUCCESS;
}

int CSystemConfigCollection::load_database_config (CIniFile &ini_reader)
{
//    SDatabaseConfig &db_config = get_database_config ();

//    // db name
//    if (FUNC_SUCCESS != load_string_item (ini_reader,
//            "DATABASE", "DSN", db_config.db_name))
//    {
//        return FUNC_FAILED;
//    }

//    // db addr
//    if (FUNC_SUCCESS != load_string_item (ini_reader,
//            "DATABASE", "DsnAddr", db_config.db_addr))
//    {
//        return FUNC_FAILED;
//    }

//    // user name
//    if (FUNC_SUCCESS != load_string_item (ini_reader,
//            "DATABASE", "UserName", db_config.user_name))
//    {
//        return FUNC_FAILED;
//    }

//    // user password
//    if (FUNC_SUCCESS != load_string_item (ini_reader,
//            "DATABASE", "Password", db_config.user_password))
//    {
//        return FUNC_FAILED;
//    }

//    // conect num
//    if (FUNC_SUCCESS != load_int_item (ini_reader,
//                                       "DATABASE",
//                                       "ConectNum",
//                                       db_config.connect_num,
//                                       1,
//                                       100))
//    {
//        return FUNC_FAILED;
//    }

    return FUNC_SUCCESS;
}

int CSystemConfigCollection::load_string_item (CIniFile &ini_reader, 
        const char *section, const char *item, std::string &str_val)
{
    std::string tmp;
    if (ini_reader.get_item_string (section, item, tmp) < 0)
    {
        printf ("读取配置项[%s::%s]失败.", section, item);
        return FUNC_FAILED;
    }

    str_val = tmp;

    return FUNC_SUCCESS;
}

int CSystemConfigCollection::load_int_item (CIniFile &ini_reader, 
    const char *section, const char *item, int &ret_val, int rang_beg, int rang_end)
{
    int tmp = -1;
    if (0 == ini_reader.get_item_int (section, item, tmp))
    {
        if (rang_beg <= tmp && tmp <= rang_end)
        {
            ret_val = tmp;
            return FUNC_SUCCESS;
        }
    }
    printf ("读取配置项[%s::%s]失败，配置项取值范围[%d, %d].",
            section, item, rang_beg, rang_end);
    
    return FUNC_FAILED;
}

