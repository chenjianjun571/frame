///************************************************************
/// @Copyright (C), 2015-2030, bafei  Information Technologies Co., Ltd.
/// @URL
/// @file           program_controller.h
/// @brief          此文件中主要定义了进程控制基类。初步实现了控制台命令解析与执行、父子进程
///                 控制、license验证、共享内存创建、进程启动主要流程控制等功能。
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月07日
/// @Description
/// @History
///************************************************************
#ifndef __PROGRAM_CONTROLLER_H_
#define __PROGRAM_CONTROLLER_H_

#include "./signal_event_handler.h"
#include "../system_public_def.h"
#include "./share_memory_manager.h"
#include <ace/Get_Opt.h>

namespace NAME_SPACE
{
    namespace frame
    {
        /// @class CProgramController
        /// @brief 各应用程序模块统一的进程控制服务接口类。主要实现了功能有：
        ///        1、控制台命令的解析、判断与执行；
        ///        2、各模块父进程（守护进程）与子进程（业务进程）的关系建立；
        ///        3、各模块lincense验证；
        ///        4、各模块父子进程间通信的共享内存的创建和管理；
        ///        5、业务进程的程序启动、停止流程控制（具体的业务流程控制由其子类来实现）；
        class CProgramController
        {
        protected:
            /// 产品名称
            std::string m_product_name;
            /// 产品版本
            std::string m_product_version;

            /// 模块名称
            std::string m_module_name;
            /// 进程名称、程序名称
            std::string m_program_name;
            /// 进程名称、程序名称, 全路径
            std::string m_program_fullname;
            /// 进程文件所在路径：全路径
            std::string m_program_fullpath;
            /// 模块配置文件名，此为相对于可执行文件所在目录的一个相对路径名
            std::string m_module_config_filename;

            /// 配置文件进程锁，以防止多个进程同时修改配置文件
            ACE_Process_Mutex m_module_config_mutex;
            /// 进程互斥量
            ACE_Process_Mutex *m_process_mutex;
            /// 进程信号捕获对象
            ACE_Sig_Handler m_sig_handler;

            /// 进程信号捕获事件处理对象
            CSignalEventHandler m_signal_event_handler;
            /// 进程以没有守护进程的方式运行
            bool m_console_mode;
            /// 显示日志
            bool m_display_log;
        public:
            /// @brief 构造函数
            CProgramController(void);

            /// @brief 析构函数
            virtual ~CProgramController(void);

            /// @brief 应用程序控制入口函数。
            /// @detail 创建进程互斥量、处理应用程序控制逻辑。此函数是提供给具体
            ///         的应用程序调用的唯一函数接口。
            /// @param[in] argc @c main函数传入的命令行参数个数
            /// @param[in] argv @c main函数传入的命令行参数列表
            /// @return FUNC_SUCCESS 操作成功, FUNC_FAILED操作失败
            int process_program_logic(int argc, ACE_TCHAR *argv[]);

            /// @brief 设置语言环境
            static int set_locale(const char *program_name);

            /// @breif 设置模块名
            /// @return NULL设置失败，否则返回设置的模块名
            inline const char* set_module_name (const char *module_name);
            /// @brief 获取模块名
            inline const char* get_module_name ()const;

            /// @brief 设置模块可执行文件名
            inline const char* set_program_name (const char *program_name);
            /// @brief 获取可执行文件名
            inline const char* get_program_name ()const;

            /// @brief 设置模块配置文件名，文件名是相对于可执行文件的一个相对路径名
            inline const char* set_module_config_filename (const char *filename);
            /// @brief 获取模块配置文件名，文件名是相对于可执行文件的一个相对路径名
            inline const char* get_module_config_filename ()const;

        protected: // ---------[业务流程控制接口原型，具体实现由业务子类实现]----------

            /// @brief shutdown_service
            /// @detail     系统业务流程关闭。纯虚函数。
            /// @return     FUNC_FAILED ,失败；FUNC_SUCCESS , 成功
            virtual int shutdown_service() = 0;

            /// @brief start_service
            /// @detail     系统业务流程启动函数。各应用程序模块可根据自己的业务流程，
            ///             在此函数中添加或者修改相关业务启动流程。
            /// @return     FUNC_FAILED ,失败；FUNC_SUCCESS , 成功。如果此函数返回
            ///             失败，应用程序可能不能启动。
            virtual int start_service() = 0;

        private:
            /// @brief 建立“短参数”和“长参数”之间的映射关系。
            /// @return FUNC_SUCCESS操作成功，FUNC_FAILED操作失败
            int create_arguments_map (ACE_Get_Opt& cmd_opt);

            /// @brief 用户输入的命令中是否含有停止命令
            /// @return
            //          -1输入的命令不合法
            //          0不含有停止命令
            //          1含有启动停止
            int is_stop_command (int argc, char **argv);

            /// @brief 用户输入的命令中是否含有启动命令
            /// @return
            //          -1输入的命令不合法
            //          0不含有启动命令
            //          1含有启动命令
            int is_start_command (int argc, char **argv);

            /// @brief set_program_switches
            /// @detail     根据命令行参数设置程序运行开关量。
            /// @param[in] argc    @c main函数传入的命令行参数个数
            /// @param[in] argv    @c main函数传入的命令行参数列表
            /// @return     FUNC_FAILED ,失败；FUNC_SUCCESS , 成功。
            int set_program_switches( int argc, ACE_TCHAR *argv[]);

            /// @brief set_program_switches
            /// @detail     根据命令行参数，控制程序执行流程
            ///
            /// @param[in] argc    @c main函数传入的命令行参数个数
            /// @param[in] argv    @c main函数传入的命令行参数列表
            /// @param[in] options 应用程序命令行中命令选项集合字符串
            /// @return     FUNC_FAILED ,失败；FUNC_SUCCESS , 成功。
            ///
            int process_program_switches(int argc, ACE_TCHAR *argv[], bool cur_locked);

            /// @brief     程序服务启动入口方法。
            /// @details 在此初始化日志记录器、共享内存；检查程序服务是否已经启动；
            ///          创建子进程服务等；
            /// @return     -1 ,失败；0 , 成功。
            int start_program(void);

            /// @brief     程序服务关闭的入口函数。
            /// @details 在此关闭日志记录器、释放共享内存、释放资源，杀死业务进程等。
            /// @return     -1 ,失败；0 , 成功。
            int shutdown_program(void);

            /// @brief     系统启动失败，杀掉进程
            /// @details
            /// @return     -1 ,失败；0 , 成功。
            int kill_allservices();

            /// @brief     设置配置文件信息。
            /// @details 暂实现了从命令行读取输入的数据库服务名、用户名、密码等，
            ///             然后分别写入到配置文件中。
            /// @return     -1 ,失败；0 , 成功。
            int set_configuration(void);

            /// @brief 显示进程帮助信息。根据输入参数，决定是否要在帮助信息前打印错误信息。
            /// @return FUNC_FAILED 失败，FUNC_SUCCESS操作成功
            int display_help_info();

            /// @brief     显示程序版本信息。
            /// @return     -1 ,失败；0 , 成功。
            int display_version_info(void);

            /// @brief     显示程序版本信息。
            /// @param[in] process_runtime_info 从共享内存读出的进程实时信息
            /// @return     1 ,失败；0 , 成功。
            int display_status_info(TProcessRuntimeInfo *process_runtime_info);

            /// @brief     读取产品名称和版本号
            /// @return     1 ,失败；0 , 成功。
            int read_product_info(void);

            /// @brief     子进程监控。
            /// @param[in] child_id 从共享内存读出的进程实时信息
            /// @return     false ,失败；true , 成功。
            bool child_process_monitor( pid_t child_id );

            /// @brief     校验系统License
            /// @return     1 成功, other: 失败
            /// @mdl_name: apm, bsm, spm, drm
            int verify_license(const char* lic_path, const char* mdl_name);

            /// @brief     销毁license
            /// @return     1 ,失败；0 , 成功。
            int dispose_license();

            /// @brief 获取可执行文件所在目录的全路径
            /// @param[out] fullpath 获取的路径
            /// @return FUNC_FAILED 获取失败，FUNC_SUCCESS获取成功
            int get_program_fullpath (std::string &fullpath);

        private:

            /// @brief 系统状态监控
            int status_monitor(void);

            ///
            /// @brief     获取进程ID以及其守护进程ID
            ///
            /// @param[out] m_pid    服务进程ID
            /// @param[out] s_pid    守护进程ID
            /// @return     -1 ,失败；0, 成功。
            int get_process_pid(pid_t* m_pid, pid_t* s_pid);
            int get_process_pid(pid_t * m_pid);

            /// @brief     设置进程ID以及其守护进程ID
            ///
            /// @param[out] m_pid    服务进程ID
            /// @param[out] s_pid    守护进程ID
            /// @return     -1 ,失败；0, 成功。
            int set_process_pid(pid_t m_pid, pid_t s_pid);
            int set_process_pid(pid_t m_pid);

            /// @brief    清理进程ID
            int free_process_pid();


            /// @brief 创建共享内存
            int create_share_memory(void);

            ///
            /// @brief     程序服务关闭的入口函数。
            /// @details 在此关闭日志记录器、释放共享内存、释放资源，杀死业务进程等。
            /// @return     -1 ,失败；0 , 成功。
            ///
            int shutdown_program(int is_kill);

            ///
            /// @brief  设置是否需要打印控制台日志，并且把当前的日志显示设备（控制台号）
            ///            设置到共享内存中。
            /// @param[in] is_open
            /// @return false ,失败；true , 成功
            int set_console_logger(bool is_open);

            ///
            /// @brief  设置需要进行状态查看的类名
            /// @param[in]    class_name    由控制台输入的 @c CDumpObject类的类名
            /// @return -1 ,失败；0 , 成功
            ///
            int set_dump_class_name(const char* class_name);

            ///
            /// @brief  设置是否需要打印控制台日志，并且把当前的日志显示设备（控制台号）
            ///            设置到共享内存中。
            /// @param[in] is_open
            /// @return false ,失败；true , 成功
            ///
            int set_logger_levels(char level_type);

            ///
            /// @brief 获取并显示当前的日志打印级别
            void get_current_logger_level();
        };

        inline const char* CProgramController::set_module_name (const char *module_name)
        {
            m_module_name = module_name;

            return get_module_name ();
        }

        inline const char* CProgramController::get_module_name ()const
        {
            return m_module_name.c_str ();
        }

        inline const char* CProgramController::set_program_name (const char *program_name)
        {
            m_program_name = program_name;

            return get_program_name ();
        }

        inline const char* CProgramController::get_program_name ()const
        {
            return m_program_name.c_str ();
        }

        inline const char* CProgramController::set_module_config_filename (const char *filename)
        {
            m_module_config_filename = filename;

            return get_module_config_filename ();
        }

        inline const char* CProgramController::get_module_config_filename ()const
        {
            return m_module_config_filename.c_str ();
        }
    }
}

#endif

