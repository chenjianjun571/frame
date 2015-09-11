
#ifndef _SHARE_MEMORY_MANAGER_H_
#define _SHARE_MEMORY_MANAGER_H_

#include "./ShareMemoryAllocator.h"


#ifdef _WIN32
#pragma pack(push,1)
#endif

#ifdef _AIX5
#pragma pack(1)
#endif

#ifdef _LINUX
#pragma pack(push,1)
#endif


_QGGROUP_BEGIN_NESTED_NAMESPACE(frame)

// 最大客户端数量
const int MAX_CLIENT_COUNT  = 150;

// 最大输出日志条数
const int MAX_LOG_COUNT     = 100;

// 日志信息最大长度
const int MAX_LOG_SIZE      = 5120;

////////////////////////////////////////////////////////////////////
// 客户端运行时信息
struct TClientRuntimeInfo
{
    char            module_type_name[64];   // 模块类型名称
    unsigned int    module_type;            // 模块类型
    char            module_address[16];     // IP 地址
    unsigned int    socket_port;            // 通讯端口
    ACE_INT64       login_time;             // 登录时间
    int             current_state;          // 当前状态
    unsigned int    send_count;             // 发送总量
    unsigned int    send_traffic;           // 发送流量
    unsigned int    receive_count;          // 接收总量
    unsigned int    receive_traffic;        // 接收流量
};

////////////////////////////////////////////////////////////////////
// 进程运行时信息
struct TProcessRuntimeInfo
{
    pid_t           pid;                    // 进程PID (long 型)
    pid_t           s_pid;                  // 守候进程PID (long 型)
    bool            run_signal;             // 运行标志
    ACE_INT64       startup_time;           // 启动时间
    ACE_INT64       heartbeat_time;         // 心跳时间

    unsigned int    current_log_index;      // 当前日志记录的下标

    char            log_info[MAX_LOG_COUNT][MAX_LOG_SIZE]; // 日志信息数组，最多可输出MAX_LOG_COUNT条日志信息

    unsigned int    server_count;           // 服务端数量
    unsigned int    server_count_current;   // 当前服务端数量
    TClientRuntimeInfo server_runtime_info[MAX_CLIENT_COUNT]; // 服务端信息

    unsigned int    client_count;           // 客户端数量
    unsigned int    client_count_current;   // 当前客户端数量
    TClientRuntimeInfo client_runtime_info[MAX_CLIENT_COUNT]; // 客户端信息

    char    stdout_tty[256];
    short     console_logger_open;
    
    /// 需要被dump的@c CDumpObject子类的类名
    char    dump_class_name[256];

    ///    用于控制@c dump_class_name是否被取走，只能取一次。
    short    is_dump;

    /// 日志打印级别
    int        log_print_level;

    /// 日志打印级别
    int        log_print_level_getted;

};


////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////
// 
class CShareMemoryManager : public CShareMemoryAllocator
{
public:
    static CShareMemoryManager* instance()
    {
        static CShareMemoryManager m_instance;

        return &m_instance;
    }
protected:
    CShareMemoryManager(void);

    virtual ~CShareMemoryManager(void);

protected:
    TProcessRuntimeInfo *m_process_runtime_info;

public:
    pid_t get_pid(void);

    pid_t get_s_pid(void);

    void set_pid(pid_t pid );

    void set_s_pid(pid_t s_pid );

    // 获得运行标志
    bool get_run_signal(void);

    // 设置运行标志
    void set_run_signal( bool run_signal );

    // 获得启动时间
    ACE_INT64 get_startup_time(void);

    // 设置启动时间
    void set_startup_time( ACE_INT64 startup_time );

    // 获得心跳时间
    ACE_INT64 get_heartbeat_time(void);

    // 设置心跳时间
    void set_heartbeat_time( ACE_INT64 heartbeat_time );

    // 取指定下标处的日志信息
    char *get_log_info( unsigned int log_index );

    // 设置客户端信息
    void set_log_info( char *log_info, int log_length );

    // 获得当前日志信息下标
    unsigned int get_log_info_index(void);

    int get_client_count(void);

    void set_client_count( int client_count );

    int get_client_count_current(void);

    void set_client_count_current( int client_count_current );

    TClientRuntimeInfo *get_client_runtime_info( unsigned int client_index );

    ///************************************************************
    /// @brief 内存池同步
    /// @details 
    /// @pre 
    /// @post 
    /// @param[in] 
    /// @param[out] 
    /// @return 
    /// @exception 
    /// @todo 
    /// @attention 
    ///************************************************************/
    int sync();

    ///************************************************************
    /// @brief 获取内存池状态
    /// @param[in] 无
    /// @return int, 小于0---共享内存区不存在，其他---共享内存存在
    ///************************************************************/
    int get_bad();

    TProcessRuntimeInfo *get_process_runtime_info(void);

public:
    /*************************************************
    Function:       init_memory_pool
    Description:    初始化共享内存池
    Input:          无      
    Output:         无
    Return:         true    成功    
                    false   失败
    *************************************************/
    virtual bool init_memory_pool();

    
    /*************************************************
    Function:       clear_memory_pool
    Description:    清除共享内存池
    Input:          无      
    Output:         无
    Return:         true    成功    
                    false   失败
    *************************************************/
    virtual bool clear_memory_pool();
public:
    /// 
    /// @brief    设置当前的显示终端
    ///    @param[in]    is_open    是否打开控制台日志显示
    ///    @param[in]    shell_tty linux下当前控制台显示终端号
    /// @return        void
    ///
    void set_process_stdout(bool is_open, char * shell_tty = NULL);

    /// 
    /// @brief    获取设置的显示终端标识
    /// @return    返回类型为@c const char* ，值为设置的显示终端标识字符串；
    ///            如果当前控制台日志显示功能未开，则返回NULL
    ///
    const char * get_process_stdout(void);
    
    /// 
    /// @brief    设置需要进行实时状态查看的类名
    ///    @param[in]    class_name    @c CDumpObject子类的类名
    /// @return        void
    ///
    void set_dump_class(const char* class_name);

    /// 
    /// @brief    获取设置的需要dump的@c CDumpObject子类对象的类名
    /// @return    返回类型为@c const char* ，值为设置的类名字符串指针；
    ///            如果该类名已经被取过一次，则返回NULL。
    ///
    const char* get_dump_class(void);

    /// 
    /// @brief    设置新的日志打印级别
    ///    @param[in]    level_type 日志级别类型
    /// @return        void
    ///
    void set_new_logger_level(int level_type);

    /// 
    /// @brief    获取设置的日志级别
    /// 
    /// @return    返回类型为@c int ，值为设置的日志级别类型；
    ///            如果该类型已经被取过一次，则返回 -1。
    ///
    int get_new_logger_level(void);

    /// 
    /// @brief 获取当前
    int get_current_logger_level();
};

// 共享内存管理器宏，使用该宏可获得 该类的唯一实例
//#define SHARE_MEMORY_MANAGER_INSTANCE ACE_Singleton< CShareMemoryManager, ACE_Recursive_Thread_Mutex >::instance()
#define SHARE_MEMORY_MANAGER_INSTANCE CShareMemoryManager::instance()

_QGGROUP_END_NESTED_NAMESPACE

#pragma pack(pop)


#endif

