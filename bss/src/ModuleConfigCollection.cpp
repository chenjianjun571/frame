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
                                          "BUS_SVR_INFO",
                                          "BUS_SVR_IP",
                                          m_net_srv_config.bus_listen_ip))
    {
        return FUNC_FAILED;
    }


    if (FUNC_SUCCESS != load_int_item(ini_reader,
                                      "BUS_SVR_INFO",
                                      "BUS_SVR_PORT",
                                      m_net_srv_config.bus_listen_port,
                                      1024,
                                      65523))
    {
        return FUNC_FAILED;
    }

    return FUNC_SUCCESS;
}

