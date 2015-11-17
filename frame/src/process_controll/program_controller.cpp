#include "./program_controller.h"
#include "../system_const_def.h"
#include "../console_resource.h"
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <glog/logging.h>

using namespace std;

extern const char* const gcv_module_config_filename;
// 全局变量
SSystemControl gv_system_control;

namespace NAME_SPACE
{
    namespace frame
    {
        CProgramController::CProgramController(void)
            : m_product_name (gcv_product_name),
              m_product_version (gcv_product_version),
              m_module_config_filename (gcv_module_config_filename),
              m_module_config_mutex (gcv_module_config_filename)
        {
        }


        CProgramController::~CProgramController(void)
        {
            if (NULL !=this->m_process_mutex)
            {
                delete this->m_process_mutex;
                this->m_process_mutex = NULL;
            }
        }

        int CProgramController::get_program_fullpath (string &fullpath)
        {
            char buffer[gcv_maxlen_filepath];
            memset (buffer, 0, sizeof(buffer));

            if (NULL != ::getcwd(buffer, sizeof(buffer)))
            {
                fullpath = buffer;
                return FUNC_SUCCESS;
            }
            return FUNC_FAILED;
        }

        int CProgramController::is_stop_command (int argc, char **argv)
        {
        //    unsigned char* p;
        //    for (int i = 1; i < argc; i++) {

        //        p = (unsigned char *) argv[i];

        //        if (*p++ != '-') {
        //            return -1;
        //        }

        //        while (*p) {
        //            switch (*p++) {
        //            // 存在停止命令返回1
        //            case 'u':
        //                return 1;
        //            }
        //        }
        //    }

        //    return 0;

            const char *argv_options = gcv_argv_options;

            ACE_Get_Opt cmd_opt (argc, argv, argv_options);
            // 建立长命令与短命令的映射关系
            if (FUNC_SUCCESS != create_arguments_map (cmd_opt))
            {
                return -1;
            }
            int arg = cmd_opt ();
            if (EOF == arg)
            {
                return -1;
            }
            do
            {
                switch (arg)
                {
                    // 存在停止命令返回1
                    case 'u':
                        return 1;
                }
                arg = cmd_opt ();
            }while (EOF != arg);

            return 0;
        }

        int CProgramController::is_start_command (int argc, char **argv)
        {
        //    unsigned char* p;
        //    for (int i = 1; i < argc; i++) {

        //        p = (unsigned char *) argv[i];

        //        if (*p++ != '-') {
        //            return -1;
        //        }

        //        while (*p) {
        //            switch (*p++) {
        //            // 存在停止命令返回1
        //            case 's':
        //                return 1;
        //            }
        //        }
        //    }

        //    return 0;

            const char *argv_options = gcv_argv_options;

            ACE_Get_Opt cmd_opt (argc, argv, argv_options);
            // 建立长命令与短命令的映射关系
            if (FUNC_SUCCESS != create_arguments_map (cmd_opt))
            {
                return -1;
            }
            int arg = cmd_opt ();
            if (EOF == arg)
            {
                return -1;
            }
            do
            {
                switch (arg)
                {
                    // 存在启动命令返回1
                    case 's':
                        return 1;
                }
                arg = cmd_opt ();
            }while (EOF != arg);

            return 0;
        }

        int CProgramController::process_program_logic(int argc, ACE_TCHAR *argv[])
        {
            // 获取进程运行绝对路径
            if (FUNC_SUCCESS != get_program_fullpath (m_program_fullpath))
            {
                cout << IDS_GET_ABS_PATH_FAILED << endl;
                return FUNC_FAILED;
            }

            // 创建进程互斥量
            m_process_mutex = new(nothrow) ACE_Process_Mutex ((this->m_program_name+"_infox_jsbn_").c_str());
            if (NULL == m_process_mutex)
            {
                cout << IDS_NEW_MEMORY_FAILED << endl;
                return FUNC_FAILED;
            }

            // 标记进程是否加锁
            bool cur_locked = false;
            // 获取进程锁
            if (0 == m_process_mutex->tryacquire ())
            {
                // 如果加锁成功，说明没有进程运行, 没有进行运行不能执行停止命令
                int ret = is_stop_command (argc, argv);
                // 如果存在停止命令
                if (1 == ret)
                {
                    m_process_mutex->release ();
                    cout << IDS_NON_SYSTEM_INSTANCE << endl;

                    return FUNC_FAILED;
                }
                // 命令解析失败
                else if (-1 == ret)
                {
                    m_process_mutex->release ();
                    cout << IDS_INVALID_COMMAND_OR_PARAM_ERROR << endl;

                    return FUNC_FAILED;
                }
                cur_locked = true;

                // 如果未启动服务时将.dat文件删除，防止出现共享内存错误
                string dat_file = this->m_program_fullpath + "/data/" + m_program_name + ".dat";
                ACE_OS::unlink(dat_file.c_str());
            }
            else
            {
                int ret = is_start_command (argc, argv);
                if (1 == ret)
                {
                    cout << IDS_PROGRAM_ALREADY_STARTED << endl;
                    return FUNC_FAILED;
                }
                // 命令解析失败
                else if (-1 == ret)
                {
                    m_process_mutex->release ();
                    cout << IDS_INVALID_COMMAND_OR_PARAM_ERROR << endl;

                    return FUNC_FAILED;
                }
            }

            // 处理 程序运行方式的配置参数
            if (FUNC_SUCCESS != set_program_switches (argc, argv))
            {
                cout << IDS_INVALID_COMMAND_OR_PARAM_ERROR << endl;
                if (cur_locked)
                {
                    m_process_mutex->release ();
                }
                return FUNC_FAILED;
            }

            // 根据命令行参数，处理程序逻辑
            if (FUNC_SUCCESS != process_program_switches (argc, argv, cur_locked))
            {
                return FUNC_FAILED;
            }
            return FUNC_SUCCESS;
        }

        int CProgramController::set_program_switches(int argc, ACE_TCHAR *argv[])
        {
            // 创建命令行参数处理对象
            ACE_Get_Opt cmd_opt(argc, argv, gcv_argv_options);

            // 建立“短参数”和“长参数”之间的映射关系；
            this->create_arguments_map(cmd_opt);

            int argument = cmd_opt();
            if (EOF == argument)
            {
                return(FUNC_FAILED);
            }

            bool b_k = false;
            bool b_d = false;
            bool b_v = false;
            bool b_h = false;
            bool b_t = false;
            bool b_s = false;
            bool b_u = false;
            bool b_c = false;
            bool b_w = false;
            bool b_l = false;

            do
            {
                switch ( argument )
                {
                    // 指定程序运行于控制台模式下
                case 'k':
                    if (b_v) return FUNC_FAILED;
                    if (b_h) return FUNC_FAILED;
                    if (b_t) return FUNC_FAILED;
                    if (b_u) return FUNC_FAILED;
                    if (b_c) return FUNC_FAILED;
                    if (b_w) return FUNC_FAILED;
                    if (b_l) return FUNC_FAILED;
                    if (b_k) return FUNC_FAILED;

                    this->m_console_mode = true;
                    b_k = true;
                    break;

                    // 在控制台屏幕上显示日志信息
                case 'd':
                    if (b_v) return FUNC_FAILED;
                    if (b_h) return FUNC_FAILED;
                    if (b_t) return FUNC_FAILED;
                    if (b_u) return FUNC_FAILED;
                    if (b_c) return FUNC_FAILED;
                    if (b_w) return FUNC_FAILED;
                    if (b_l) return FUNC_FAILED;
                    if (b_d) return FUNC_FAILED;

                    this->m_display_log = true;
                    b_d = true;
                    break;

                case 'v':
                    if (b_k) return FUNC_FAILED;
                    if (b_d) return FUNC_FAILED;
                    if (b_h) return FUNC_FAILED;
                    if (b_t) return FUNC_FAILED;
                    if (b_s) return FUNC_FAILED;
                    if (b_u) return FUNC_FAILED;
                    if (b_c) return FUNC_FAILED;
                    if (b_w) return FUNC_FAILED;
                    if (b_l) return FUNC_FAILED;
                    if (b_v) return FUNC_FAILED;
                    b_v = true;
                    break;

                case 'h':
                    if (b_k) return FUNC_FAILED;
                    if (b_d) return FUNC_FAILED;
                    if (b_v) return FUNC_FAILED;
                    if (b_t) return FUNC_FAILED;
                    if (b_s) return FUNC_FAILED;
                    if (b_u) return FUNC_FAILED;
                    if (b_c) return FUNC_FAILED;
                    if (b_w) return FUNC_FAILED;
                    if (b_l) return FUNC_FAILED;
                    if (b_h) return FUNC_FAILED;
                    b_h = true;
                    break;

                case 't':
                    if (b_k) return FUNC_FAILED;
                    if (b_d) return FUNC_FAILED;
                    if (b_v) return FUNC_FAILED;
                    if (b_h) return FUNC_FAILED;
                    if (b_s) return FUNC_FAILED;
                    if (b_u) return FUNC_FAILED;
                    if (b_c) return FUNC_FAILED;
                    if (b_w) return FUNC_FAILED;
                    if (b_l) return FUNC_FAILED;
                    if (b_t) return FUNC_FAILED;
                    b_t = true;
                    break;

                case 's':
                    if (b_v) return FUNC_FAILED;
                    if (b_h) return FUNC_FAILED;
                    if (b_t) return FUNC_FAILED;
                    if (b_u) return FUNC_FAILED;
                    if (b_c) return FUNC_FAILED;
                    if (b_w) return FUNC_FAILED;
                    if (b_l) return FUNC_FAILED;
                    if (b_s) return FUNC_FAILED;
                    b_s = true;
                    break;

                case 'u':
                    if (b_k) return FUNC_FAILED;
                    if (b_d) return FUNC_FAILED;
                    if (b_v) return FUNC_FAILED;
                    if (b_h) return FUNC_FAILED;
                    if (b_t) return FUNC_FAILED;
                    if (b_s) return FUNC_FAILED;
                    if (b_c) return FUNC_FAILED;
                    if (b_w) return FUNC_FAILED;
                    if (b_l) return FUNC_FAILED;
                    if (b_u) return FUNC_FAILED;
                    b_u = true;
                    break;

                case 'c':
                    if (b_k) return FUNC_FAILED;
                    if (b_d) return FUNC_FAILED;
                    if (b_v) return FUNC_FAILED;
                    if (b_h) return FUNC_FAILED;
                    if (b_t) return FUNC_FAILED;
                    if (b_s) return FUNC_FAILED;
                    if (b_u) return FUNC_FAILED;
                    if (b_w) return FUNC_FAILED;
                    if (b_l) return FUNC_FAILED;
                    if (b_c) return FUNC_FAILED;
                    b_c = true;
                    break;

                case 'w':
                    if (b_k) return FUNC_FAILED;
                    if (b_d) return FUNC_FAILED;
                    if (b_v) return FUNC_FAILED;
                    if (b_h) return FUNC_FAILED;
                    if (b_t) return FUNC_FAILED;
                    if (b_s) return FUNC_FAILED;
                    if (b_u) return FUNC_FAILED;
                    if (b_c) return FUNC_FAILED;
                    if (b_l) return FUNC_FAILED;
                    if (b_w) return FUNC_FAILED;
                    b_w = true;
                    break;
                case 'l':
                    if (b_k) return FUNC_FAILED;
                    if (b_d) return FUNC_FAILED;
                    if (b_v) return FUNC_FAILED;
                    if (b_h) return FUNC_FAILED;
                    if (b_t) return FUNC_FAILED;
                    if (b_s) return FUNC_FAILED;
                    if (b_u) return FUNC_FAILED;
                    if (b_c) return FUNC_FAILED;
                    if (b_w) return FUNC_FAILED;
                    if (b_l) return FUNC_FAILED;
                    b_l = true;
                    break;
                default:
                    return FUNC_FAILED;
                }

                argument = cmd_opt();

            }while(EOF != argument);

            return FUNC_SUCCESS;
        }

        int CProgramController::process_program_switches(int argc, ACE_TCHAR *argv[], bool cur_locked)
        {
            // 创建命令行参数处理对象
            ACE_Get_Opt cmd_opt(argc, argv, gcv_argv_options);

            // 建立“短参数”和“长参数”之间的映射关系；
            this->create_arguments_map(cmd_opt);

            int argument = cmd_opt();
            if (argument == EOF)
            {
                if (cur_locked) m_process_mutex->release ();
                return(FUNC_FAILED);
            }

            do
            {
                switch (argument)
                {
                case 's':
                    // 启动程序服务
                    this->start_program();
                    break;

                case 'u':
                    // 终止程序
                    m_module_config_mutex.acquire();
                    this->shutdown_program(KILL_PROCESS);
                    m_module_config_mutex.release();
                    // 程序终止后，需要释放进程锁
                    m_process_mutex->release ();
                    return FUNC_SUCCESS;

                case 'c':
                    // 设置程序配置文件
                    this->set_configuration();
                    if (cur_locked) m_process_mutex->release ();
                    break;

                case 't':
                    // 显示程序运行状态
                    this->status_monitor();
                    break;

                case 'h':
                    // 显示帮助信息
                    this->display_help_info ();
                    if (cur_locked) m_process_mutex->release ();
                    return FUNC_SUCCESS;

                case 'v':
                    // 显示版本信息
                    this->display_version_info();

                    return FUNC_SUCCESS;
                case 'w':
                    {
                        this->set_console_logger(true);

                        char* dump_class_name = cmd_opt.opt_arg();
                        if (NULL == dump_class_name)
                        {
                            cout << IDS_INVALID_COMMAND_OR_PARAM_ERROR << endl;

                            this->display_help_info();
                        }
                        this->set_dump_class_name(dump_class_name);

                        if (cur_locked) m_process_mutex->release ();
                    }
                    break;
                case 'l':
                    {
                        char* log_level = cmd_opt.opt_arg();
                        if (log_level == NULL || strlen(log_level) > 1)
                        {
                            cout<<IDS_INVALID_LOG_OPER_PARAM<<endl;
                            this->display_help_info();
                        }
                        else
                        {
                            if (log_level[0] == 's')
                            {
                                this->get_current_logger_level();
                            }
                            else
                            {
                                this->set_logger_levels(log_level[0]);
                            }
                        }
                    }
                    break;
                case ':':
                    ACE_ERROR_RETURN(( LM_ERROR, ACE_TEXT("-%c requires an argument\n"), cmd_opt.opt_opt() ), -1 );
                default:
                    break;
                }

                argument = cmd_opt();
            }
            while( argument != EOF );

            return FUNC_SUCCESS;
        }

        int CProgramController::create_arguments_map( ACE_Get_Opt& cmd_opt )
        {
            if (cmd_opt.long_option( ACE_TEXT("startup"), 's', ACE_Get_Opt::NO_ARG ) == -1)
            {
                return(FUNC_FAILED);
            }

            if (cmd_opt.long_option( ACE_TEXT("shutdown"), 'u', ACE_Get_Opt::NO_ARG ) == -1)
            {
                return(FUNC_FAILED);
            }

            if (cmd_opt.long_option( ACE_TEXT("config"), 'c', ACE_Get_Opt::ARG_OPTIONAL ) == -1)
            {
                return(FUNC_FAILED);
            }

            if (cmd_opt.long_option( ACE_TEXT("console"), 'k', ACE_Get_Opt::NO_ARG ) == -1)
            {
                return(FUNC_FAILED);
            }

            if (cmd_opt.long_option( ACE_TEXT("display"), 'd', ACE_Get_Opt::NO_ARG ) == -1)
            {
                return(FUNC_FAILED);
            }

            if (cmd_opt.long_option( ACE_TEXT("help"), 'h', ACE_Get_Opt::NO_ARG ) == -1)
            {
                return(FUNC_FAILED);
            }

            if (cmd_opt.long_option( ACE_TEXT("version"), 'v', ACE_Get_Opt::NO_ARG ) == -1)
            {
                return(FUNC_FAILED);
            }

            if (cmd_opt.long_option( ACE_TEXT("status"), 't', ACE_Get_Opt::NO_ARG ) == -1)
            {
                return(FUNC_FAILED);
            }

            if (cmd_opt.long_option( ACE_TEXT("watch"), 'w', ACE_Get_Opt::ARG_REQUIRED) == -1)
            {
                return(FUNC_FAILED);
            }

            if (cmd_opt.long_option( ACE_TEXT("loglevel"), 'l', ACE_Get_Opt::ARG_REQUIRED) == -1)
            {
                return(FUNC_FAILED);
            }

            return 0;
        }


        int CProgramController::set_configuration(void)
        {
            return 0;
        }

        int CProgramController::start_program(void)
        {
            int result = 0;

            ////////////////////////////////////////////////////////////////////////
            // 读取产品信息
            result = this->read_product_info();
            if ( result != 0 )
            {
                // 如果读取产品信息失败，给出提示信息
                cout << IDS_SYSTEM_FILE_BAD << endl;

                return(1);
            }

            CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

            //////////////////////////////////////////////////////////////////////////
            // 创建并初始化共享内存
            result = this->create_share_memory();

            if ( result != 0 )
            {
                return 1;
            }

            // 检查进程是否已启动，正在运行
            ACE_INT64 time_sub    = 0;
            time_sub = ACE_OS::time(NULL) - share_memory_manager->get_heartbeat_time();

            if ( share_memory_manager->get_startup_time() != 0 && time_sub < 10 && time_sub > -10 )
            {
                // 如果进程已启动，正在运行中
                return 1;
            }

            ACE_Time_Value wait_timer( 5 );

            //////////////////////////////////////////////////////////////////////////
            // 根据用户输入的 命令行参数，
            // 确定是以 Daemon 进程的方式启动，还是运行于控制台下
            // 该特性只在 UNIX 和 Linux 下起作用，Win32不受影响
            if ( this->m_console_mode == false )
            {
                //////////////////////////////////////////////////////////////////////////
                // 以 Daemon 进程的方式启动进程

                share_memory_manager->clear_memory_pool();
                share_memory_manager->release_memory_pool();

                // PID
                pid_t pid = 0;

                // 创建子进程
                pid = ACE_OS::fork();

                //设置子进程PID
                SHARE_MEMORY_MANAGER_INSTANCE->set_s_pid(pid);

                if ( pid == -1 )
                {
                    // fork 失败
                    return -1;
                }
                else if ( pid != 0 )
                {
                    // 父进程, 退出进程
                    return 0;
                }

                //
                ACE_OS::setsid(); // Become session leader.

                ACE_OS::signal( SIGHUP, SIG_IGN );

                bool child_result = true;

                while ( child_result == true )
                {
                    // 通过 fork 再创建新的进程
                    pid = ACE_OS::fork();

                    if ( pid != 0 )
                    {

                        // 等待业务进程退出
                        if (ACE_OS::waitpid( pid, NULL, 0) != pid )
                        {
                            return 1;
                        }

                        child_result = this->child_process_monitor( pid );

                        if ( child_result == true )
                        {
                            ACE_OS::sleep(2);
                            continue;
                        }

                        return 0;
                    }
                    else
                    {
                        // 运行系统核心服务的子进程
                        // 进入以下的核心业务代码的处理
                        this->create_share_memory();

                        break;
                    }
                }
            }

            if (this->m_console_mode == false)
            {
                this->m_process_mutex->acquire();
            }

            share_memory_manager->set_startup_time( ACE_OS::time(NULL) );
            share_memory_manager->set_heartbeat_time( ACE_OS::time(NULL) );

            //设置核心服务进程PID
            share_memory_manager->set_pid(ACE_OS::getpid());
            share_memory_manager->set_s_pid(ACE_OS::getppid());
            set_process_pid(share_memory_manager->get_pid(),share_memory_manager->get_s_pid());

            // 启动核心服务
            result = this->start_service();
            if ( result != 0 )
            {
                LOG(ERROR)<<"系统启动失败.";
                // 关闭核心服务
                this->kill_allservices();

                if (this->m_console_mode == false)
                {
                    this->m_process_mutex->release();
                }

                // 释放共享内存区
                share_memory_manager->clear_memory_pool();
                share_memory_manager->release_memory_pool();

                return result;
            }
            else
            {
                LOG(INFO)<<"系统启动成功.";
            }

            try
            {
                ACE_Time_Value waiter(0, 50000);

                while ( share_memory_manager->get_startup_time() != 0 )
                {
                    share_memory_manager->set_heartbeat_time(ACE_OS::time(NULL));
                    ACE_OS::sleep( waiter );
                }
            }
            catch(...)
            {}

            // 关闭核心服务
            this->shutdown_service();

            ACE_OS::sleep(1);

            // 释放共享内存区
            share_memory_manager->clear_memory_pool();
            share_memory_manager->release_memory_pool();

            if (this->m_console_mode == false)
            {
                this->m_process_mutex->release();
            }

            raise(SIGTERM);

            LOG(INFO)<<"系统关闭成功.";

            return(FUNC_SUCCESS);
        }


        bool CProgramController::child_process_monitor(pid_t child_id)
        {
            //bool result = false;

            ACE_exitcode exit_code;

            // 等待进程退出
            pid_t pid = ACE_OS::waitpid(child_id, &exit_code);

            CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

            return share_memory_manager->is_back_store_file_create();
        }

        int CProgramController::shutdown_program(void)
        {
            // 创建共享内存管理对象
            CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

            // 组合生成存放后备存储文件的路径
            char back_store_file[256]={0};
            //     修改文件路径为绝对路径
            ACE_OS::memset(back_store_file, 0, sizeof(back_store_file));

            ACE_OS::snprintf(back_store_file, sizeof(back_store_file),
                             "%s/data/%s.dat",
                             this->m_program_fullpath.c_str(),
                             this->m_program_name.c_str() );


            share_memory_manager->set_share_memory_name( this->m_program_name );
            share_memory_manager->set_back_store_file( back_store_file );

            // 创建共享内存池
            if ( share_memory_manager->create_memory_pool() == false )
            {
                // 共享内存池创建失败
                //ACE_DEBUG(( LM_INFO, "%s\n", IDS_SHARE_MEMORY_CREATE_FAILED ));
                LOG(ERROR)<<IDS_SHARE_MEMORY_CREATE_FAILED;
                return 1;
            }

            // 初始化共享内存池
            if ( share_memory_manager->init_memory_pool() == false )
            {
                // 内存池初始化失败
                //ACE_DEBUG(( LM_INFO, "%s\n", IDS_SHARE_MEMORY_INIT_FAILED ));
                LOG(ERROR)<<IDS_SHARE_MEMORY_INIT_FAILED;
                return 1;
            }

            // 检查进程是否已启动，正在运行
            ACE_INT64 time_sub    = 0;
            time_sub = ACE_OS::time(NULL) - share_memory_manager->get_heartbeat_time();

            if (share_memory_manager->get_startup_time() != 0 && time_sub < 10 && time_sub > -10 )
            {
                // 当进程已启动时，重置其启动时间，等待关闭进程
                share_memory_manager->set_startup_time( 0 );
            }
            else
            {
                if ( share_memory_manager->get_startup_time() != 0 )
                {
                }

                //ACE_DEBUG(( LM_INFO, "%s\n", IDS_NON_SYSTEM_INSTANCE ));
                LOG(ERROR)<<IDS_NON_SYSTEM_INSTANCE;
            }

            // 释放共享内存池
            // 判断共享内存文件是否已成功释放，如释放，说明服务进程已成功退出，
            // 则可以退出循环，结束本进程
            for ( int i = 0; i < 10; i++ )
            {
                if ( share_memory_manager->is_back_store_file_create() == true )
                {
                    ACE_OS::sleep(1);
                    continue;
                }
                else
                {
                    break;
                }
            }

            LOG(INFO)<<"系统关闭成功";

            return 0;
        }


        int CProgramController::status_monitor(void)
        {
            // 创建共享内存管理对象
            CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

            // 组合生成存放后备存储文件的路径
            char back_store_file[255] = {0};

            //     修改文件路径为绝对路径
            ACE_OS::memset(back_store_file, 0, sizeof(back_store_file));

            ACE_OS::snprintf(back_store_file, sizeof(back_store_file),
                             "%s/data/%s.dat",
                             this->m_program_fullpath.c_str(),
                             this->m_program_name.c_str() );

            share_memory_manager->set_share_memory_name( this->m_program_name );

            share_memory_manager->set_back_store_file( back_store_file );

            // 创建共享内存池
            if ( share_memory_manager->create_memory_pool() == false )
            {
                // 共享内存池创建失败
                //ACE_DEBUG(( LM_INFO, "%s\n", IDS_SHARE_MEMORY_CREATE_FAILED ));
                LOG(INFO)<<IDS_SHARE_MEMORY_CREATE_FAILED;

                return 1;
            }

            // 初始化共享内存池
            if ( share_memory_manager->init_memory_pool() == false )
            {
                // 内存池初始化失败
                //ACE_DEBUG(( LM_INFO, "%s\n", IDS_SHARE_MEMORY_INIT_FAILED ));
                LOG(INFO)<<IDS_SHARE_MEMORY_INIT_FAILED;

                return 1;
            }

            // 检查进程是否已启动，正在运行
            ACE_INT64 time_sub    = 0;
            time_sub = ACE_OS::time(NULL) - share_memory_manager->get_heartbeat_time();

            if (share_memory_manager->get_startup_time() != 0 && time_sub < 10 && time_sub > -10 )
            {
                // 当进程已启动时，重置其启动时间，等待关闭进程
                TProcessRuntimeInfo *proces_runtime_info = share_memory_manager->get_process_runtime_info();

                this->display_status_info( proces_runtime_info );
            }
            else
            {
                if ( share_memory_manager->get_startup_time() != 0 )
                {
                    //share_memory_manager->clear_memory_pool();
                }

                //ACE_DEBUG(( LM_INFO, "%s\n", IDS_NON_SYSTEM_INSTANCE ));
                LOG(INFO)<<IDS_NON_SYSTEM_INSTANCE;
            }

            // 释放共享内存池
            //share_memory_manager->clear_memory_pool();
            //share_memory_manager->release_memory_pool();

            ACE_OS::sleep(10);

            return 0;
        }


        int CProgramController::display_status_info(TProcessRuntimeInfo *process_runtime_info)
        {
            // 读取产品信息
            if (this->read_product_info() != 0 )
            {
                return(1);
            }

            return 0;
        }

        int CProgramController::read_product_info(void)
        {
            return(0);
        }

        int CProgramController::display_help_info ()
        {
            char parameter_filename[gcv_maxlen_filepath];
            ACE_OS::memset(parameter_filename, 0, sizeof(parameter_filename));

            // 参数信息存放的文件路径
            ACE_OS::snprintf(parameter_filename, sizeof(parameter_filename),
                    "%s/%s", m_program_fullpath.c_str(), gcv_paraminfo_filename);

            // 打开系统版本信息文件
            fstream file_reader(parameter_filename, ios_base::in);
            if (!file_reader)
            {
                char strerr[gcv_maxlen_error_info];
                memset (strerr, 0, sizeof(strerr));

                strerror_r (ACE_OS::last_error (), strerr, sizeof(strerr));

                printf (IDS_OPENFILE_FAILED_STRERROR, parameter_filename, strerr);

                return(FUNC_FAILED);
            }

            // 显示程序模块的名称
            char str_title[gcv_maxlen_filepath];
            ACE_OS::memset(str_title, 0, sizeof(str_title));

            ACE_OS::snprintf(str_title, sizeof(str_title),
                    IDS_PROMPT_SYSTEM_HELPINFO, this->m_program_name.c_str());

            cout << endl<< str_title << endl;

            cout << file_reader.rdbuf();

            cout << endl;

            file_reader.close();

            return(FUNC_SUCCESS);
        }

        int CProgramController::display_version_info(void)
        {
            //char antispam_file_name[255];
            char verinfo_filename[gcv_maxlen_filepath];
            ACE_OS::memset(verinfo_filename, 0, sizeof(verinfo_filename));

            ACE_OS::snprintf(verinfo_filename, sizeof(verinfo_filename),
                    "%s/%s", this->m_program_fullpath.c_str(), gcv_versioninfo_filename );

            // 打开系统版本信息文件
            fstream file_reader(verinfo_filename, ios_base::in);

            if (!file_reader)
            {
                char strerr[gcv_maxlen_error_info];
                memset (strerr, 0, sizeof(strerr));

                strerror_r (ACE_OS::last_error (), strerr, sizeof(strerr));
                printf (IDS_OPENFILE_FAILED_STRERROR, verinfo_filename, strerr);

                return(FUNC_FAILED);
            }

            char file_string[256];
            ACE_OS::memset(file_string, 0, sizeof(file_string));

            // 显示产品产品名称
            file_reader.getline (file_string, sizeof(file_string));
            cout << file_string << endl;

            // 显示版本号
            file_reader.getline (file_string, sizeof(file_string));
            cout << file_string << endl;

            // 显示程序模块的名称
            cout << m_module_name << endl;

            // 读取，并显示文件剩余内容
            cout << file_reader.rdbuf();
            cout << endl;

            file_reader.close();

            return(FUNC_SUCCESS);
        }

        int CProgramController::dispose_license()
        {
            return 0;
        }


        int CProgramController::verify_license(const char* lic_path, const char* mdl_name)
        {
            return FUNC_SUCCESS;
        }

        int CProgramController::set_locale(const char *program_name)
        {
            char *loc = NULL;

            loc = setlocale( LC_ALL, "" );

            if (NULL == loc)
            {
                return FUNC_FAILED;
            }


            char *domain_name = NULL;

            // 设置message的位置和文件名
            ACE_TCHAR buffer[1024];

            ACE_OS::getcwd(buffer, sizeof(buffer));

            string base_path = buffer;

            domain_name = bindtextdomain(program_name, (base_path + "/locale").c_str());

            if ( domain_name == NULL )
            {
                return FUNC_FAILED;
            }

            domain_name = textdomain(program_name);

            if ( domain_name == NULL )
            {
                return FUNC_FAILED;
            }

            return FUNC_SUCCESS;
        }


        int CProgramController::get_process_pid(pid_t* m_pid, pid_t* s_pid)
        {
            string file_name = "tmp/"+this->m_program_name+".pid";

            FILE *fp = NULL;

            fp = fopen(file_name.c_str(),"rb");

            if(fp == NULL)
                return -1;

            char c_pid[128]={0};

            fgets(c_pid,sizeof(c_pid)-1, fp);

            if (strlen(c_pid)>0)
            {
                *m_pid = atol(c_pid);
            }

            memset(c_pid,0,sizeof(c_pid));

            fgets(c_pid, sizeof(c_pid)-1, fp);

            if (strlen(c_pid) > 0)
            {
                *s_pid = atol(c_pid);
            }

            if (*m_pid <= 0 || *s_pid <= 0)
            {
                fclose(fp);
                return -1;
            }
            else
            {
                fclose(fp);
                return 0;
            }
        }

        int CProgramController::get_process_pid(pid_t * m_pid)
        {
            string file_name = "tmp/"+this->m_program_name+".pid";

            FILE *fp = NULL;

            fp = fopen(file_name.c_str(),"rb");

            if(fp == NULL)
                return -1;

            char c_pid[128]={0};

            fgets(c_pid,sizeof(c_pid)-1,fp);

            if (strlen(c_pid) > 0)
            {
                *m_pid = atol(c_pid);
            }

            if (*m_pid <= 0)
            {
                fclose(fp);
                return -1;
            }
            else
            {
                fclose(fp);
                return 0;
            }
        }

        int CProgramController::set_process_pid(pid_t m_pid,pid_t s_pid)
        {
            if (m_pid <= 0 || s_pid <= 0)
            {
                return -1;
            }

            ACE_OS::mkdir("./tmp");

            string file_name = "tmp/"+this->m_program_name+".pid";

            FILE *fp = NULL;

            fp = fopen(file_name.c_str(),"w+b");

            if(fp == NULL)
                return -1;

            char c_pid[128] = {0};

            sprintf(c_pid,"%ld\n\0",m_pid);

            fwrite(c_pid,strlen(c_pid),1,fp);

            sprintf(c_pid,"%ld\n\0",s_pid);

            fwrite(c_pid,strlen(c_pid),1,fp);

            fclose(fp);

            return 0;
        }

        int CProgramController::set_process_pid(pid_t m_pid)
        {
            if (m_pid <= 0)
            {
                return -1;
            }

            ACE_OS::mkdir("./tmp");

            string file_name = "tmp/"+this->m_program_name+".pid";

            FILE *fp = NULL;

            fp = fopen(file_name.c_str(),"w+b");

            if(fp == NULL)
                return -1;

            char c_pid[128] = {0};

            sprintf(c_pid,"%ld\n\0",m_pid);

            fwrite(c_pid,strlen(c_pid),1,fp);

            fclose(fp);

            return 0;
        }

        int CProgramController::free_process_pid()
        {
            string file_name = "tmp/"+this->m_program_name+".pid";

            unlink(file_name.c_str());

            return 0;
        }

        int CProgramController::create_share_memory(void)
        {
            CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

            // 检查后备文件存放路径是否有效，若无效，则创建相应文件路径
            ACE_DIR *directory = ACE_OS::opendir( "data" );

            if ( directory == NULL )
            {
                // 以 ACE 默认的目录操作权限，创建日志存放目录
                ACE_OS::mkdir( "data", ACE_DEFAULT_DIR_PERMS );

                // 检查路径是否
                directory = ACE_OS::opendir( "data" );

                if ( directory == NULL )
                {
                    // 创建后备文件存储路径失败，
                    ACE_DEBUG(( LM_INFO, "%s \n", IDS_BACKSTORE_FILE_CREATE_FAILED ));

                    return -1;
                }
            }

            ACE_OS::closedir( directory );

            // 组合生成存放后备存储文件的路径
            char back_store_file[255] = {0};

            //     修改文件路径为绝对路径
            ACE_OS::memset(back_store_file, 0, sizeof(back_store_file));

            ACE_OS::snprintf(back_store_file, sizeof(back_store_file),
                             "%s/data/%s.dat", this->m_program_fullpath.c_str(), this->m_program_name.c_str());



            share_memory_manager->set_share_memory_name( this->m_program_name );
            share_memory_manager->set_back_store_file( back_store_file );

            // 创建共享内存池
            if ( share_memory_manager->create_memory_pool() == false )
            {
                // 创建共享内存池失败
                ACE_DEBUG(( LM_INFO, "%s \n", IDS_SHARE_MEMORY_CREATE_FAILED ));

                return -2;
            }

            // 初始化共享内存池
            if ( share_memory_manager->init_memory_pool() == false )
            {
                // 初始化共享内存池失败
                ACE_DEBUG(( LM_INFO, "%s \n", IDS_SHARE_MEMORY_INIT_FAILED ));

                return -3;
            }

            return 0;
        }

        int CProgramController::shutdown_program(int is_kill)
        {
            // 组合生成存放后备存储文件的路径
            char back_store_file[256] = {0};
            sprintf( back_store_file,"%s/data/%s.dat", this->m_program_fullpath.c_str(), this->m_program_name.c_str() );

            bool is_dat_close = false;

            if(this->m_process_mutex->tryacquire()==0)
            {
                free_process_pid();
                ACE_OS::unlink(back_store_file);
                cout << IDS_NON_SYSTEM_INSTANCE << endl;
                return -1;
            }


            pid_t m_pid = 0, s_pid = 0;


            if(get_process_pid(&m_pid,&s_pid) < 0)
            {
                is_dat_close = true;
            }
            else
            {
                if(this->m_process_mutex->tryacquire() != 0)
                {
                    ACE::terminate_process(s_pid);
                    ACE::terminate_process(m_pid);
                    free_process_pid();
                }
                else
                {
                    free_process_pid();

                    ACE_OS::unlink(back_store_file);

                    cout << IDS_NON_SYSTEM_INSTANCE << endl;

                    return -1;
                }
            }

            // 创建共享内存管理对象
            CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

            if (is_dat_close == true)
            {
                share_memory_manager->set_share_memory_name( this->m_program_name );
                share_memory_manager->set_back_store_file( back_store_file );

                // 创建共享内存池
                if ( share_memory_manager->create_memory_pool() == false )
                {
                    // 共享内存池创建失败
                    ACE_DEBUG(( LM_INFO, "%s\n", IDS_SHARE_MEMORY_CREATE_FAILED ));
                    return 1;
                }

                // 初始化共享内存池
                if ( share_memory_manager->init_memory_pool() == false )
                {
                    // 内存池初始化失败
                    ACE_DEBUG(( LM_INFO, "%s\n", IDS_SHARE_MEMORY_INIT_FAILED ));
                    return 1;
                }

                if (share_memory_manager == NULL)
                {
                    cout << IDS_NON_SYSTEM_INSTANCE << endl;
                    return -1;
                }

                share_memory_manager->set_startup_time(0);
            }

            LOG(INFO)<<IDS_PROMPT_SYSTEM_SHUTDOWNSUCCESS;

            //删除程序dat文件
            ACE_OS::sleep(ACE_Time_Value(3));

            ACE_OS::unlink(back_store_file);

            char temp_cmd[1024] = {0};
            unsigned int sigkey = ACE::crc32(ACE::basename(back_store_file,ACE_DIRECTORY_SEPARATOR_CHAR));
            snprintf(temp_cmd, 1024, "ipcs -s | grep %x | awk '{print $2}' | xargs ipcrm -s", sigkey);
            system(temp_cmd);

            cout << IDS_PROMPT_SYSTEM_SHUTDOWNSUCCESS << endl;

            return 0;
        }

        int CProgramController::set_console_logger(bool is_open)
        {
            // 创建共享内存管理对象
            CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

            // 组合生成存放后备存储文件的路径
            char back_store_file[256]={0};
            sprintf( back_store_file,"%s/data/%s.dat", this->m_program_fullpath.c_str(), this->m_program_name.c_str() );

            share_memory_manager->set_share_memory_name( this->m_program_name );
            share_memory_manager->set_back_store_file( back_store_file );

            // 创建共享内存池
            if ( share_memory_manager->create_memory_pool() == false )
            {
                // 共享内存池创建失败
                ACE_DEBUG(( LM_INFO, "%s\n", IDS_SHARE_MEMORY_CREATE_FAILED ));

                return 1;
            }

            // 初始化共享内存池
            if ( share_memory_manager->init_memory_pool() == false )
            {
                // 内存池初始化失败
                ACE_DEBUG(( LM_INFO, "%s\n", IDS_SHARE_MEMORY_INIT_FAILED ));

                return 1;
            }

            // 检查进程是否已启动，正在运行
            ACE_INT64 time_sub    = 0;
            time_sub = ACE_OS::time(NULL) - share_memory_manager->get_heartbeat_time();

            ACE_OS::system("tty >tty.dat");
            // 打开刚刚重定向的文件
            fstream file_reader("tty.dat", ios_base::in);
            if ( !file_reader )
            {
                return 1;
            }

            char file_string[256];
            ACE_OS::memset(file_string, 0, sizeof(file_string));

            // 读取终端数据
            file_reader.getline( file_string, 256 );
            //printf("This tty is: %s\n", file_string);

            // 向共享内存写入新终端数据
            share_memory_manager->set_process_stdout(is_open, file_string);
            ACE_OS::system("rm -f tty.dat");

            return 0;
        }



        int CProgramController::set_dump_class_name(const char* class_name)
        {
            // 创建共享内存管理对象
            CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

            // 组合生成存放后备存储文件的路径
            char back_store_file[256]={0};
            sprintf( back_store_file,"%s/data/%s.dat", this->m_program_fullpath.c_str(), this->m_program_name.c_str() );

            share_memory_manager->set_share_memory_name( this->m_program_name );
            share_memory_manager->set_back_store_file( back_store_file );

            // 创建共享内存池
            if ( share_memory_manager->create_memory_pool() == false )
            {
                // 共享内存池创建失败
                ACE_DEBUG(( LM_INFO, "%s\n", IDS_SHARE_MEMORY_CREATE_FAILED ));

                return -1;
            }

            // 初始化共享内存池
            if ( share_memory_manager->init_memory_pool() == false )
            {
                // 内存池初始化失败
                ACE_DEBUG(( LM_INFO, "%s\n", IDS_SHARE_MEMORY_INIT_FAILED ));

                return -1;
            }

            // 检查进程是否已启动，正在运行
            ACE_INT64 time_sub    = 0;
            time_sub = ACE_OS::time(NULL) - share_memory_manager->get_heartbeat_time();

            if ( share_memory_manager->get_startup_time() != 0 )
            {
                // 如果进程已启动，正在运行中
                share_memory_manager->set_dump_class(class_name);
            }
            else
            {
                ACE_DEBUG((LM_WARNING, IDS_NO_PROCESS_RUNNING_COMMAND_INVALID, 'w'));
            }

            return 0;
        }

        int CProgramController::set_logger_levels(char level_type)
        {
            return 0;
        }

        void CProgramController::get_current_logger_level()
        {
            if (this->create_share_memory() != 0)
            {
                return ;
            }

            // 创建共享内存管理对象
            CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

            // 检查进程是否已启动，正在运行
            ACE_INT64 time_sub    = 0;
            time_sub = ACE_OS::time(NULL) - share_memory_manager->get_heartbeat_time();

            if ( share_memory_manager->get_startup_time() == 0 )
            {
                ACE_DEBUG((LM_WARNING, IDS_NO_PROCESS_RUNNING_COMMAND_INVALID, 'l'));

                return ;
            }
        }

        int CProgramController::kill_allservices()
        {
            char back_store_file[256];
            memset (back_store_file, 0, sizeof(back_store_file));
            sprintf( back_store_file,"%s/data/%s.dat",
                    this->m_program_fullpath.c_str(), this->m_program_name.c_str() );

            if(this->m_process_mutex->tryacquire()==0)
            {
                free_process_pid();
                ACE_OS::unlink(back_store_file);
                return -1;
            }

            CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

            if(share_memory_manager != NULL)
            {
                share_memory_manager->clear_memory_pool();
                share_memory_manager->release_memory_pool();
            }


            ACE_OS::unlink(back_store_file);

            pid_t m_pid = 0, s_pid = 0;

            get_process_pid(&m_pid,&s_pid);

            free_process_pid();

            ACE::terminate_process(s_pid);
            ACE::terminate_process(m_pid);

            return 0;
        }
    }
}
