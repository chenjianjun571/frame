///***************************************************************************
/// @Copyright (C), 1993-2010, bafei  Information Technologies Co., Ltd.
///
/// @file ModuleController.h
///
/// @brief 具体模块的业务逻辑控制对象，继承自@c CProgramController，主要实现了@c CProgramController
///           类的两个纯虚函数。
///
/// @attention
///
/// @todo
///
/// @version v1.0
///
/// @author mone
///
/// @date 2010/06/25
///
/// @note 
///        <author>        <time>            <version >        <desc>    
///
///**************************************************************************
#ifndef INFOX_BUSSINESS_SERVICE
#define INFOX_BUSSINESS_SERVICE

#include "./ProgramController.h"

/// 类型向前申明
class CBusinessControl;
class CSystemConfigCollection; 

_QGGROUP_BEGIN_NESTED_NAMESPACE(frame)
/// @class CModuleController
/// @brief 继承于@c CProgramController接口类。主要实现了@c CProgramController类的两个
///        纯虚函数，实现具体的业务流程控制，以配合@c CProgramController类控制进程
///        的启动与停止。
class CModuleController : public CProgramController
{
public:

    /// 默认构造函数    
    CModuleController(void);


    /// 默认析构函数    
    ~CModuleController(void);
public:

    /// @brief    产生自身唯一实例的静态函数
    static CModuleController* get_instance();

    /// @brief 设置系统配置信息集
    void register_system_config_collection (CSystemConfigCollection *sys_config);

    /// @brief 设置系统业务控制对象
    void register_business_controler (CBusinessControl *business_ctrl);

private:// ---------[各业务模块具体实现]----------
    /// @brief shutdown_service
    /// @detail     系统业务流程关闭。
    /// @return        FUNC_FAILED ,失败；FUNC_SUCCESS , 成功
    int shutdown_service();

    /// @brief start_service
    /// @detail     系统业务流程启动函数。各应用程序模块可根据自己的业务流程，
    ///             在此函数中添加或者修改相关业务启动流程。
    /// @return     FUNC_FAILED ,失败；FUNC_SUCCESS , 成功。如果此函数返回
    ///             失败，应用程序可能不能启动。
    int start_service();

    /// @brief 检查磁盘空间是否满足进程运行需要
    /// @details 如果传入的参数为 0 ，则使用系统定义的常量
    /// @param[in] space_size 进程需要的磁盘空间
    /// @return false, 不足；true, 磁盘空间够用
    bool check_disk_free_space(size_t space_size);
private:
    /// @c CModuleController类自身唯一的单体实例
    static CModuleController *m_instance;

    /// 业务逻辑控制对象
    CBusinessControl *m_business_control;

    /// 系统配置信息
    CSystemConfigCollection *m_sys_config;
};

#define MODULE_CONTROLER (CModuleController::get_instance ())

_QGGROUP_END_NESTED_NAMESPACE

#endif

