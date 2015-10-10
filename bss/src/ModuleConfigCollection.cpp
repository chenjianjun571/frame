#include "ModuleConfigCollection.h"

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
                                          "ISS_SVR_INFO",
                                          "ISS_SVR_IP",
                                          m_net_srv_config.iss_service_listen_ip))
    {
        return FUNC_FAILED;
    }


    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "ISS_SVR_INFO",
                                      "ISS_SVR_PORT",
                                      m_net_srv_config.iss_service_listen_port,
                                      1024,
                                      65523))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "ISS_SVR_INFO",
                                      "ISS_HEARTBEAT_TIME",
                                      m_net_srv_config.iss_service_heartbeat_detection,
                                      5,
                                      6000))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_string_item (ini_reader,
                                          "TSS_SVR_INFO",
                                          "TSS_SVR_IP",
                                          m_net_srv_config.tss_service_listen_ip))
    {
        return FUNC_FAILED;
    }


    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "TSS_SVR_INFO",
                                      "TSS_SVR_PORT",
                                      m_net_srv_config.tss_service_listen_port,
                                      1024,
                                      65523))
    {
        return FUNC_FAILED;
    }

    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "TSS_SVR_INFO",
                                      "TSS_HEARTBEAT_TIME",
                                      m_net_srv_config.tss_service_heartbeat_detection,
                                      5,
                                      6000))
    {
        return FUNC_FAILED;
    }

    return FUNC_SUCCESS;
}

