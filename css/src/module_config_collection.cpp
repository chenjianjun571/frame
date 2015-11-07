#include "module_config_collection.h"

CModuleConfigCollection::CModuleConfigCollection(void)
{
}

CModuleConfigCollection::~CModuleConfigCollection(void)
{
}

CModuleConfigCollection* CModuleConfigCollection::get_instance ()
{
    static CModuleConfigCollection *_instance = NULL;
    if (NULL == _instance)
    {
        _instance = new(std::nothrow) CModuleConfigCollection;
    }

    return _instance;
}

int CModuleConfigCollection::dump(const char* first_param /* = NULL */,
                                   const char* second_param /* = NULL */)
{
    CSystemConfigCollection::dump ();

    // 实现模块私有的dump信息

    return 0;
}

int CModuleConfigCollection::load_module_file_config (CIniFile &ini_reader)
{
    if (FUNC_SUCCESS != load_string_item (ini_reader,
                                          "HOST_INFO",
                                          "HOST_IP",
                                          m_srv_config.host_ip))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "HOST_INFO",
                                      "DATA_PROC_THREAD_NUM",
                                      m_srv_config.data_proc_thread_num,
                                      1,
                                      50))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "BSS_SVR_INFO",
                                      "BSS_SVR_PORT",
                                      m_srv_config.bss_service_listen_port,
                                      1024,
                                      65523))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "BSS_SVR_INFO",
                                      "BSS_HEARTBEAT_TIME",
                                      m_srv_config.bss_service_heartbeat_detection,
                                      5,
                                      6000))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "RPC_INFO",
                                      "RPC_SRV_PORT",
                                      m_srv_config.rpc_listen_port,
                                      1024,
                                      65535))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "RPC_INFO",
                                      "PRC_PROC_THREAD_NUM",
                                      m_srv_config.rpc_proc_thread_num,
                                      1,
                                      100))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_string_item (ini_reader,
                                          "DB_INFO",
                                          "DB_HOST",
                                          m_srv_config.db_host))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "DB_INFO",
                                      "DB_PORT",
                                      m_srv_config.db_port,
                                      1024,
                                      65535))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_string_item (ini_reader,
                                          "DB_INFO",
                                          "DB_NAME",
                                          m_srv_config.db_name))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_string_item (ini_reader,
                                          "DB_INFO",
                                          "DB_USER",
                                          m_srv_config.db_user))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_string_item (ini_reader,
                                          "DB_INFO",
                                          "DB_PASSWORD",
                                          m_srv_config.db_password))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "DB_INFO",
                                      "DB_MAX_CONNECT_NUM",
                                      m_srv_config.db_max_con_num,
                                      1,
                                      100))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "CMS_SVR_INFO",
                                      "CMS_SVR_PORT",
                                      m_srv_config.cms_service_listen_port,
                                      1024,
                                      65523))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "CMS_SVR_INFO",
                                      "CMS_HEARTBEAT_TIME",
                                      m_srv_config.cms_service_heartbeat_detection,
                                      5,
                                      6000))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "SMS_SVR_INFO",
                                      "SMS_SVR_PORT",
                                      m_srv_config.sms_service_listen_port,
                                      1024,
                                      65523))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "SMS_SVR_INFO",
                                      "SMS_HEARTBEAT_TIME",
                                      m_srv_config.sms_service_heartbeat_detection,
                                      5,
                                      6000))
    {
        return FUNC_FAILED;
    }









    return FUNC_SUCCESS;
}

