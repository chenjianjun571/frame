///************************************************************
/// @Copyright (C), 2015-2030, bafei  Information Technologies Co., Ltd.
/// @URL
/// @file           module_controller.cpp
/// @brief          具体模块的业务逻辑控制对象，继承自@c CProgramController，
///                 主要实现了@c CProgramController类的两个纯虚函数。
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月07日
/// @Description
/// @History
///************************************************************
#include "./module_controller.h"
#include "../system_const_def.h"
#include "./business_control.h"
#include "../console_resource.h"
#include "../config/system_config_collection.h"
#include <glog/logging.h>

extern SSystemControl    gv_system_control;
extern ACE_Process_Mutex g_ini_mutex;

namespace NAME_SPACE
{
    namespace frame
    {
        CModuleController *CModuleController::m_instance = nullptr;

        CModuleController::CModuleController(void)
        {
            m_business_control = NULL;
            m_sys_config = NULL;
        }

        CModuleController::~CModuleController(void)
        {
        }

        /// @brief    产生自身唯一实例的静态函数
        CModuleController* CModuleController::get_instance()
        {
            if (NULL == m_instance)
            {
                m_instance = new(std::nothrow) CModuleController ();
            }
            return(m_instance);
        }

        void CModuleController::register_system_config_collection (CSystemConfigCollection *sys_config)
        {
            m_sys_config = sys_config;
        }


        void CModuleController::register_business_controler (CBusinessControl *business_ctrl)
        {
            m_business_control = business_ctrl;
        }

        int CModuleController::start_service()
        {
            // 注册 系统消息处理事件
            this->m_signal_event_handler.set_sig_handler( &this->m_sig_handler );
            this->m_signal_event_handler.set_program_name( this->m_program_name );

            this->m_sig_handler.register_handler( SIGPIPE, &this->m_signal_event_handler );

        #ifdef _DEBUG_STACK
            //修改信号处理函数
            this->m_sig_handler.register_handler( SIGABRT, &this->m_signal_event_handler );
            this->m_sig_handler.register_handler( SIGSEGV, &this->m_signal_event_handler );
            this->m_sig_handler.register_handler( SIGBUS,  &this->m_signal_event_handler );
            this->m_sig_handler.register_handler( SIGQUIT, &this->m_signal_event_handler );
            this->m_sig_handler.register_handler( SIGILL,  &this->m_signal_event_handler );
            this->m_sig_handler.register_handler( SIGTRAP, &this->m_signal_event_handler );
            this->m_sig_handler.register_handler( SIGFPE,  &this->m_signal_event_handler );
            this->m_sig_handler.register_handler( SIGEMT,  &this->m_signal_event_handler );
            this->m_sig_handler.register_handler( SIGSYS,  &this->m_signal_event_handler );
            this->m_sig_handler.register_handler( SIGXCPU, &this->m_signal_event_handler );
            this->m_sig_handler.register_handler( SIGXFSZ, &this->m_signal_event_handler );
        #endif

            // 检测磁盘空间是否可用
            if (this->check_disk_free_space(0) == false)
            {
                return FUNC_FAILED;
            }

            // 读取配置文件
            if (FUNC_SUCCESS != m_sys_config->load_module_file_config (
                        m_module_config_filename.c_str (), m_module_config_mutex))
            {
                printf ("读取模块配置文件(%s)失败.\n", m_module_config_filename.c_str ());
                return FUNC_FAILED;
            }

            // 启动业务服务
            if (FUNC_SUCCESS != m_business_control->start_system_control ())
            {
                return(FUNC_FAILED);
            }

            return(FUNC_SUCCESS);
        }

        int CModuleController::shutdown_service()
        {
            // 解除注册的信号
            this->m_sig_handler.remove_handler(SIGPIPE);

        #ifdef _DEBUG_STACK
            this->m_sig_handler.remove_handler(SIGABRT);
            this->m_sig_handler.remove_handler(SIGSEGV);
            this->m_sig_handler.remove_handler(SIGBUS);
            this->m_sig_handler.remove_handler(SIGQUIT);
             this->m_sig_handler.remove_handler(SIGILL);
            this->m_sig_handler.remove_handler(SIGTRAP);
             this->m_sig_handler.remove_handler(SIGFPE);
            this->m_sig_handler.remove_handler(SIGEMT);
            this->m_sig_handler.remove_handler(SIGSYS);
            this->m_sig_handler.remove_handler(SIGXCPU);
            this->m_sig_handler.remove_handler(SIGXFSZ);
        #endif

            // 置标志位使线程终止
            gv_system_control.sys_status = enStoped;

            m_module_config_mutex.acquire ();

            m_business_control->stop_business_service ();

            m_module_config_mutex.release ();

            ACE_OS::sleep(1);

            return(FUNC_SUCCESS);
        }


        bool CModuleController::check_disk_free_space(size_t space_size)
        {
            if(space_size == 0)
            {
                space_size = MIN_DISK_SPACE_SIZE;
            }

            struct statfs disk_space_info;
            memset(&disk_space_info, 0, sizeof(disk_space_info));
            char str_buf[256] = {0};
            char * program_path = NULL;

            // 获取当前进程运行路径
            program_path = ACE_OS::getcwd(str_buf, sizeof(str_buf)-1);

            // 如果获取路径失败设置默认目录
            if (program_path == NULL || strlen(program_path) == 0)
            {
                strcpy(str_buf, "/home/antispam");
            }

            int rel = statfs(program_path, &disk_space_info);

            if (rel != 0)
            {
                // 获取磁盘空间失败
                LOG(ERROR)<<"获取磁盘空间失败，错误码:"<< rel<<".";

                return false;
            }

            if (rel == 0 && (((disk_space_info.f_bavail * disk_space_info.f_bsize) / 1024 / 1024) < space_size))
            {
                LOG(ERROR)<<"程序运行磁盘空间不足 "<<MIN_DISK_SPACE_SIZE<<" MB，仅仅剩余 "<<((disk_space_info.f_bavail * disk_space_info.f_bsize) / 1024 / 1024)<<"MB.";
                return false;
            }

            return true;
        }
    }
}
