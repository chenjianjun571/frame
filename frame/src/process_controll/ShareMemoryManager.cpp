#include "./ShareMemoryManager.h"

using namespace jsbn::frame;

CShareMemoryManager::CShareMemoryManager(void) : m_process_runtime_info(NULL)
{
    //
}

CShareMemoryManager::~CShareMemoryManager(void)
{
    //
	m_process_runtime_info = NULL;
	m_share_memory = NULL;
}

pid_t CShareMemoryManager::get_pid(void)
{
	if (get_bad() < 0)
	{
		return -1;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        return this->m_process_runtime_info->pid;
    }
    else
    {
        return -1;
    }
}

pid_t CShareMemoryManager::get_s_pid(void)
{
	if (get_bad() < 0)
	{
		return -1;
	}
	if ( this->m_process_runtime_info != NULL)
	{
		return this->m_process_runtime_info->s_pid;
	}
	else
	{
		return -1;
	}
}

void CShareMemoryManager::set_pid(pid_t pid)
{
	if (get_bad() < 0)
	{
		return;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        this->m_process_runtime_info->pid = pid;
    }
}

void CShareMemoryManager::set_s_pid(pid_t s_pid)
{
	if (get_bad() < 0)
	{
		return;
	}
	if ( this->m_process_runtime_info != NULL)
	{
		this->m_process_runtime_info->s_pid = s_pid;
	}
}

// 获得运行标志
bool CShareMemoryManager::get_run_signal(void)
{
	if (get_bad() < 0)
	{
		return false;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        return this->m_process_runtime_info->run_signal;
    }
    else
    {
        return false;
    }
}

// 设置运行标志
void CShareMemoryManager::set_run_signal( bool run_signal )
{
	if (get_bad() < 0)
	{
		return;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        this->m_process_runtime_info->run_signal = run_signal;
    }
}

// 获得启动时间
ACE_INT64 CShareMemoryManager::get_startup_time(void)
{
	if (get_bad() < 0)
	{
		return 0;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        return this->m_process_runtime_info->startup_time;
    }
    else
    {
        return 0;
    }
}

// 设置启动时间
void CShareMemoryManager::set_startup_time( ACE_INT64 startup_time )
{
	if (get_bad() < 0)
	{
		return;
	}
    if ( this->m_process_runtime_info != NULL )
    {
        this->m_process_runtime_info->startup_time = startup_time;
    }
}

// 获得心跳时间
ACE_INT64 CShareMemoryManager::get_heartbeat_time(void)
{
	if (get_bad() < 0)
	{
		return 0;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        return this->m_process_runtime_info->heartbeat_time;
    }
    else
    {
        return 0;
    }
}


// 设置心跳时间
void CShareMemoryManager::set_heartbeat_time( ACE_INT64 heartbeat_time )
{
	if (get_bad() < 0)
	{
		return;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        this->m_process_runtime_info->heartbeat_time = heartbeat_time;
    }
}

// 取指定下标处的日志信息
char *CShareMemoryManager::get_log_info( unsigned int log_index )
{
	if (get_bad() < 0)
	{
		return NULL;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        if ( log_index >= MAX_LOG_COUNT )
        {
            return NULL;
        }

        return this->m_process_runtime_info->log_info[log_index];
    }
    else
    {
        return NULL;
    }

}

// 设置客户端信息
void CShareMemoryManager::set_log_info( char *log_info, int log_length )
{
	if (get_bad() < 0)
	{
		return;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        // 下标加1
        if ( this->m_process_runtime_info->current_log_index == MAX_LOG_COUNT - 1 )
        {
            this->m_process_runtime_info->current_log_index = 0;
        }
        else
        {
            this->m_process_runtime_info->current_log_index++;
        }

		ACE_OS::memset( this->m_process_runtime_info->log_info[this->m_process_runtime_info->current_log_index], 0, sizeof(this->m_process_runtime_info->log_info) );
        ACE_OS::memcpy( this->m_process_runtime_info->log_info[this->m_process_runtime_info->current_log_index], log_info, log_length );
    }
}

// 获得当前日志信息下标
unsigned int CShareMemoryManager::get_log_info_index(void)
{
	if (get_bad() < 0)
	{
		return 0;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        return this->m_process_runtime_info->current_log_index;
    }
    else
    {
        return 0;
    }

}




int CShareMemoryManager::get_client_count(void)
{
	if (get_bad() < 0)
	{
		return 0;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        return this->m_process_runtime_info->client_count;
    }
    else
    {
        return 0;
    }
}


void CShareMemoryManager::set_client_count( int client_count )
{   
	if (get_bad() < 0)
	{
		return;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        this->m_process_runtime_info->client_count = client_count;
    }
}


int CShareMemoryManager::get_client_count_current(void)
{
	if (get_bad() < 0)
	{
		return 0;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        return this->m_process_runtime_info->client_count_current;
    }
    else
    {
        return 0;
    }
}


void CShareMemoryManager::set_client_count_current( int current_count )
{
	if (get_bad() < 0)
	{
		return;
	}
    if ( this->m_process_runtime_info != NULL)
    {
        this->m_process_runtime_info->client_count_current = current_count;
    }
}


TClientRuntimeInfo *CShareMemoryManager::get_client_runtime_info( unsigned int client_index )
{
	if (get_bad() < 0)
	{
		return NULL;
	}
    if ( this->m_process_runtime_info == NULL)
    {
        return NULL;
    }

    if (client_index >= MAX_CLIENT_COUNT )
    {
        return NULL;
    }
    else
    {
        return &this->m_process_runtime_info->client_runtime_info[client_index];
    }

}


TProcessRuntimeInfo *CShareMemoryManager::get_process_runtime_info(void)
{
	if (get_bad() < 0)
	{
		return NULL;
	}
    if ( this->m_process_runtime_info == NULL)
    {
        return NULL;
    }

    return this->m_process_runtime_info;
}



/*************************************************
Function:       init_memory_pool
Description:    初始化共享内存池
Input:          无      
Output:         无
Return:         true    成功    
                false   失败
*************************************************/
bool CShareMemoryManager::init_memory_pool()
{
    bool result = false;

    result = CShareMemoryAllocator::init_memory_pool();

    if ( result == false )
    {
        return false;
    }
    else
    {
		this->m_process_runtime_info = static_cast<TProcessRuntimeInfo*>(this->m_share_memory);
        return true;
    }
}

/*************************************************
Function:       clear_memory_pool
Description:    清除共享内存池
Input:          无      
Output:         无
Return:         true    成功    
false   失败
*************************************************/
bool CShareMemoryManager::clear_memory_pool()
{
    bool result = false;

    result = CShareMemoryAllocator::clear_memory_pool();

    if ( result == false )
    {
        return false;
    }
    else
    {
        this->m_process_runtime_info = NULL;

        return true;
    }

}

int CShareMemoryManager::get_bad()
{
	int res = -1;
	if (m_memory_pool)
	{
		res = this->m_memory_pool->avail_chunks(200);
	}

	return res;
}

int CShareMemoryManager::sync()
{
	int res = -1;
	if (m_memory_pool)
	{
		res = this->m_memory_pool->sync();
	}

	return res;
}

/// 
/// @brief	设置当前的显示终端
///	@param[in]	is_open	是否打开控制台日志显示
///	@param[in]	shell_tty linux下当前控制台显示终端号
/// @return		void
///
void CShareMemoryManager::set_process_stdout(bool is_open, char * shell_tty)
{
	if (get_bad() < 0)
	{
		return;
	}
	if (this->m_process_runtime_info == NULL)
	{
		return;
	}

	if (is_open)
	{
		if (shell_tty == NULL)
		{
			this->m_process_runtime_info->console_logger_open = 1;
			return;
		}

		this->m_process_runtime_info->console_logger_open = 0;
		ACE_OS::memset(this->m_process_runtime_info->stdout_tty, 0, sizeof(this->m_process_runtime_info->stdout_tty));
		ACE_OS::memcpy(this->m_process_runtime_info->stdout_tty, shell_tty, strlen(shell_tty));
	}
	else
	{
		this->m_process_runtime_info->console_logger_open = 1;
	}
}

/// 
/// @brief	获取设置的显示终端标识
/// @return	返回类型为@c const char* ，值为设置的显示终端标识字符串；
///			如果当前控制台日志显示功能未开，则返回NULL
///
const char * CShareMemoryManager::get_process_stdout()
{
	if (get_bad() < 0)
	{
		return NULL;
	}
	if (this->m_process_runtime_info == NULL)
	{
		return NULL;
	}

	//	我们认为当console_logger_open时为控制台日志显示器开启
	if ( this->m_process_runtime_info->console_logger_open == 0 )
	{
		return this->m_process_runtime_info->stdout_tty;
	}
	else
	{
		return NULL;
	}
}

/// 
/// @brief	设置需要进行实时状态查看的类名
///	@param[in]	class_name	@c CDumpObject子类的类名
/// @return		void
///
void CShareMemoryManager::set_dump_class(const char* class_name)
{
	if (get_bad() < 0)
	{
		return;
	}
	if (class_name == NULL)
	{
		return ;
	}

	if (this->m_process_runtime_info == NULL)
	{
		return ;
	}

	ACE_OS::memset(this->m_process_runtime_info->dump_class_name, 0, 
		sizeof(this->m_process_runtime_info->dump_class_name));

	ACE_OS::strsncpy(this->m_process_runtime_info->dump_class_name, class_name,
		sizeof(this->m_process_runtime_info->dump_class_name) - 1);

	//	设置is_dump = 1，表示有一个新的未取走的类名
	this->m_process_runtime_info->is_dump = 1;
}

/// 
/// @brief	获取设置的需要dump的@c CDumpObject子类对象的类名
/// @return	返回类型为@c const char* ，值为设置的类名字符串指针；
///			如果该类名已经被取过一次，则返回NULL。
///
const char* CShareMemoryManager::get_dump_class()
{
	if (get_bad() < 0)
	{
		return NULL;
	}
	if (this->m_process_runtime_info == NULL)
	{
		return NULL;
	}

	// 我们认为当is_dump为0时表示已经被取走，或者还未设置任何类名
	if (this->m_process_runtime_info->is_dump == 0)
	{
		return NULL;
	}

	this->m_process_runtime_info->is_dump = 0;
	
	return this->m_process_runtime_info->dump_class_name;
}


/// 
/// @brief	设置需要进行实时状态查看的类名
///	@param[in]	class_name	@c CDumpObject子类的类名
/// @return		void
///
void CShareMemoryManager::set_new_logger_level(int level_type)
{
	if (get_bad() < 0)
	{
		return ;
	}
	if (this->m_process_runtime_info == NULL)
	{
		return ;
	}
	
	//	设置日志级别
	this->m_process_runtime_info->log_print_level = level_type;

	//	设置log_print_level_getted = 1，表示有一个新的未取走的类名
	this->m_process_runtime_info->log_print_level_getted = 1;
}

int CShareMemoryManager::get_new_logger_level()
{
	if (get_bad() < 0)
	{
		return -1;
	}
	if (this->m_process_runtime_info == NULL)
	{
		return -1;
	}

	// 我们认为当log_print_level_getted为0时，
	// 表示已经被取走，或者没有动态设置过新的日志打印级别
	if (this->m_process_runtime_info->log_print_level_getted != 1)
	{
		return -1;
	}

	//	设置log_print_level_getted = -1，表示该级别已经被取走
	this->m_process_runtime_info->log_print_level_getted = -1;

	return this->m_process_runtime_info->log_print_level;
}

int CShareMemoryManager::get_current_logger_level()
{
	if (get_bad() < 0)
	{
		return -1;
	}
	if (this->m_process_runtime_info == NULL)
	{
		return -1;
	}

	// 如果 log_print_level_getted = 0 表示该日志级别为初始值，即“一般信息”
	if (m_process_runtime_info->log_print_level_getted == 0)
	{
		return 3;
	}

	return this->m_process_runtime_info->log_print_level;
}

