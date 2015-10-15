#include "module_business_control.h"
#include "module_config_collection.h"
#include "module_const_def.h"
#include <new>

#include "./communicate/communicate_service.h"

using namespace std;
using namespace jsbn;

CModuleBusinessControl* CModuleBusinessControl::get_instance ()
{
    static CModuleBusinessControl m_instance;
    return &m_instance;
}

int CModuleBusinessControl::init_business_info()
{
    CBusinessControl::init_business_info();

    // 添加各模块私有的初始化信息///////////////////////////////////////////
    // 启动网络事件框架
    if (NetFrame::Instance()->NetWorkInit() != FUNC_SUCCESS) {
            LOG(ERROR)<<"网络初期化失败....";
            return FUNC_FAILED;
    }

    OBJ_MONITOR->open(NULL);

    return FUNC_SUCCESS;
}

void CModuleBusinessControl::cleanup_resource()
{
    CBusinessControl::cleanup_resource();

    // 关闭网络
    NetFrame::Instance()->NetWorkExit();
}

bool CModuleBusinessControl::create_operate_object()
{
    /********************** 创建并挂接模块的子业务*************************/
    // 挂载通讯服务
    CommunicateService *comm_service = new (std::nothrow) CommunicateService;
    if (NULL == comm_service) return false;
    this->register_operator(comm_service);

    return true;
}


