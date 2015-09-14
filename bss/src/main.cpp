///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           main.cpp
/// @brief          程序启动入口
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2014年12月06日
/// @Description
/// @History
///************************************************************
#include "ModuleConstDef.h"
#include "ModuleConfigCollection.h"
#include "ModuleBusinessControl.h"
#include <iostream>
#include <unistd.h>
#include <glog/logging.h>

extern const char* const gcv_module_config_filename = MODULE_CONFIG_FILENAME;

using namespace std;
using jsbn::frame::CModuleController;
using jsbn::frame::CProgramController;

class SignalHandle {
public:
    static void handle(const char* data, int size)
    {
        std::ofstream fs("dump.log",std::ios::app);
        std::string str = std::string(data,size);
        fs<<str;
        fs.close();
        LOG(ERROR)<<str;
    }
};

/// @brief 程序主函数入口
int main(int argc, char* argv[])
{
    // 设置工作区路径
  char exePath[gcv_maxlen_filepath];
  char fullPath[gcv_maxlen_filepath];

  memset (exePath, 0, sizeof(exePath));
  memset (fullPath, 0, sizeof(fullPath));

  sprintf(exePath, "/proc/%d/exe", getpid());

  readlink(exePath, fullPath, 1024);

  string strFilePath = fullPath;

  int nPos = strFilePath.find_last_of('/');

  fullPath[nPos] = 0;

  ::chdir(fullPath);

    //---------------------------------------------
    // Description:    在Unix系统下，如果send在等待协
    //              议传送数据时网络断开的话，调用
    //              send的进程会接收到一个SIGPIPE信
    //              号，进程对该信号的默认处理是进程
    //              终止。在Unix系统下，如果recv函数
    //              在等待协议接收数据时网络断开了，
    //              那么调用recv的进程会接收到一个
    //              SIGPIPE信号，进程对该信号的默认
    //              处理是进程终止。
    //              处理方法：在初始化时调用
    //              signal(SIGPIPE,SIG_IGN)忽略该信号
    //              （只需一次）其时send或recv函数将
    //              返回-1，errno为EPIPE，可视情况关
    //              闭socket或其他处理
    //----------------------------------------------

    signal(SIGPIPE, SIG_IGN);

    //ACE::init();

    if ( FUNC_SUCCESS != CProgramController::set_locale(gcv_program_name))
    {
        cout<<""<<endl;
        cout<<"Because the language environment variable is error, "
              "so the operation failed."
            <<endl;
        cout<<"Please reset the language environment variable, "
            "and try again!"<<endl;
        cout<<""<<endl;
        cout<<"PS:"<<endl;
        cout<<"     Chinese: export LANG=zh_CN.UTF-8"<<endl;
        cout<<"     English: export LANG=en_US.UTF-8"<<endl;
        cout<<""<<endl;

        //ACE::fini();
        return (FUNC_FAILED);
    }

    // 设置日志模块
    google::InitGoogleLogging(argv[0]);
    FLAGS_colorlogtostderr=true;
    google::SetLogDestination(google::INFO,"log/info_");
    google::SetLogDestination(google::WARNING,"log/warning_");
    google::SetLogDestination(google::ERROR,"log/error_");
    google::SetLogDestination(google::FATAL,"log/fatal_");
    FLAGS_stderrthreshold=google::INFO;// 输出到标准输出的时候大于 INFO 级别的都输出
    FLAGS_logbufsecs =0;//实时输出日志
    FLAGS_max_log_size =20;//最大日志大小（MB）
    // 默认捕捉 SIGSEGV 信号信息输出会输出到 stderr，可以通过下面的方法自定义输出方式
    google::InstallFailureSignalHandler();
    google::InstallFailureWriter(&SignalHandle::handle);

    // 设置模块名
    CModuleController::get_instance()->set_module_name (gcv_module_name);
    // 设置模块可执行文件名
    CModuleController::get_instance()->set_program_name (gcv_program_name);

    // 注册配置信息集
    CModuleController::get_instance()->register_system_config_collection (SYS_CONFIG);
    // 注册业务控制器
    CModuleController::get_instance()->register_business_controler (MODULE_BUSINESS_CONTROLER);

    // 启动进程
    if (FUNC_FAILED == CModuleController::get_instance()->process_program_logic (argc, argv))
    {
    }

    //ACE::fini();

    return(FUNC_SUCCESS);

}

