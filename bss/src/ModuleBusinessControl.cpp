#include "ModuleBusinessControl.h"
#include "ModuleConfigCollection.h"
#include "ModuleConstDef.h"
#include <new>

using namespace std;

CModuleBusinessControl* CModuleBusinessControl::get_instance ()
{
    static CModuleBusinessControl m_instance;
    return &m_instance;
}

int CModuleBusinessControl::init_business_info()
{
    CBusinessControl::init_business_info();
//    // 添加各模块私有的初始化信息///////////////////////////////////////////
//    // 启动网络事件框架
//    if(vv_net_work_init() != FUNC_SUCCESS)
//    {
//        return FUNC_FAILED;
//    }

    OBJ_MONITOR->open(NULL);

    return FUNC_SUCCESS;
}

void CModuleBusinessControl::cleanup_resource()
{
    CBusinessControl::cleanup_resource();
//    // 添加模块的清理处理流程/////////////////////////////////////////////
//    if(vv_net_work_exit() != FUNC_SUCCESS)
//    {
//        return;
//    }
}

bool CModuleBusinessControl::create_operate_object()
{
//    /********************** 创建并挂接模块的子业务*************************/
//    // 挂载通讯服务
//    CCommunicateService *comm_service = new (std::nothrow) CCommunicateService;
//    if (NULL == comm_service) return false;
//    this->register_operator(comm_service);

    return true;
}


