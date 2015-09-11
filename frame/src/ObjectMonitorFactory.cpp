/// *****************************************************************
/// @file	ObjectMonitorFactory.cpp
///
/// @warning	Copyright (C), 1993-2010, bafei  Information Technologies Co., Ltd.
///
/// @brief		此文件中定义了模块内所有继承了状态查看接口的对象维护以及状态打印类。
///
/// @attention  
///
/// @todo    
///
/// @version	1.0
///
/// @author		mone
///
/// @date		2010-04-30
///
/// @note	    <author>	<date>			<note>  \n       
/// *****************************************************************
#include "./ObjectMonitorFactory.h"
#include "./process_controll/ShareMemoryManager.h"
#include "./ConsoleResource.h"

using namespace jsbn::frame;

using std::string;

CObjectMonitorFactory::CObjectMonitorFactory():
m_is_running(true),
m_std_out(NULL)
{
}

CObjectMonitorFactory::~CObjectMonitorFactory(void)
{
    if (true == m_is_running)
    {
        fini();
    }
	m_std_out = NULL;
}


int CObjectMonitorFactory::open(void *args )
{
	m_is_running = true;
	
	return this->activate();
}

int CObjectMonitorFactory::fini()
{
	this->m_is_running = false;

	this->wait();

	return 0;
}

/// 
/// @brief	添加需要被监控的@c CDumpObject 类对象。
/// @details 在添加的时候，采用类名作为键值，建立类名与类对象的映射，
///			 如果类名相同的类对象，把它加到同一个映射后的链表上。
///	@param[in]	dump_object	新的@c CDumpObject类对象指针。
/// @return		false ,失败；true , 成功
///
bool CObjectMonitorFactory::attach_dump_object(CDumpObject *dump_object)
{
	if (dump_object == NULL)
	{
		return false;
	}

	ACE_Guard<ACE_Thread_Mutex> guard(m_objects_mutex);

	// 添加该对象到@c CDumpObject对象容器中
	// 这里用到了map容器的[]运算符的特殊性：如果不存在该容器的键key，则默认添加一个,
	// 并返回新添加的该key对应的value；如果存在了一个key，则直接返回该key的value
	// 
	this->m_dump_object_map[dump_object->get_class_name()].push_back(dump_object);

	return true;
}

bool CObjectMonitorFactory::dettach_dump_object(CDumpObject *dump_object)
{
	if (dump_object == NULL)
	{
		return false;
	}

	ACE_Guard<ACE_Thread_Mutex> guard(m_objects_mutex);

	// 先找到此对象对应的map节点
	TDumpObjectMap::iterator map_iter;
	map_iter = m_dump_object_map.find(dump_object->get_class_name());
	if (map_iter == m_dump_object_map.end())
	{
		return false;
	}

	// 再尝试从map节点上的对象链表上查找当前需要删除的对象
	TDumpObjectList::iterator list_iter;
	TDumpObjectList& object_list = (*map_iter).second;
	for (list_iter = object_list.begin(); list_iter != object_list.end(); ++list_iter)
	{
		if ((*list_iter) == dump_object)
		{
			object_list.erase(list_iter);
			break;
		}
	}

	return true;
}

///	@brief 主动对象类的各个线程入口
int CObjectMonitorFactory::svc()
{
	this->watch_dump_object_thread();

	return 0;
}

/// 
/// @brief	实时查看@c CDumpObject 类对象状态的线程函数。
/// @details 定时循环查询共享内存中是否有外部发起的查看某个@c CDumpObject的类
///			 对象的命令。如果有，则找到此类名对应的类对象，打印出该对象的内部
///			 状态；如果没有，则继续循环判断。
/// @return		void
///
void CObjectMonitorFactory::watch_dump_object_thread()
{
	while (this->m_is_running)
	{
		const char* first_param = NULL;
		const char* second_param = NULL;

		CShareMemoryManager *share_memory_manager = SHARE_MEMORY_MANAGER_INSTANCE;

		int logger_level = share_memory_manager->get_new_logger_level();
        if ( logger_level != -1){}

		const char * class_name = share_memory_manager->get_dump_class();
		if ( class_name == NULL )
		{
			ACE_OS::sleep(1);
			continue;
		}

		const char * tty_path = share_memory_manager->get_process_stdout();
		if ( tty_path != NULL )
		{
			if (this->m_std_out != NULL)
			{
				ACE_OS::fclose(m_std_out);
				m_std_out = NULL;
			}

			this->m_std_out = ACE_OS::freopen(tty_path, "w", stdout);
		}
		else
		{
			ACE_OS::sleep(1);
			continue;
		}

		string tmp_dump_str(class_name);
		string::size_type tmp_pos = 0;
		
		class_name = tmp_dump_str.c_str();
		tmp_pos = tmp_dump_str.find_first_of("+", tmp_pos);
		if (tmp_pos != string::npos)
		{
			tmp_dump_str[tmp_pos] = '\0';
			first_param = class_name + tmp_pos + 1;

			tmp_pos = tmp_dump_str.find_first_of("+", tmp_pos + 1);
			if (tmp_pos != string::npos)
			{
				tmp_dump_str[tmp_pos] = '\0';
				second_param = class_name + tmp_pos + 1;
			}
		}

		// 如果类名为"all"，表示用户想要打印出所有注册了的对象信息。
		if (strcmp(class_name, "all") == 0)
		{
			this->print_all_class_names();
		}

		// 否则，先查找对应的具体类对象
		else
		{
			// 查看是否存在该类名所对应的类对象处于被监控队列中
			TDumpObjectMap::iterator map_iter = 
				this->m_dump_object_map.find(class_name);
			if (map_iter == this->m_dump_object_map.end())
			{
				printf(IDS_OBJ_MONITOR_CLASS_NAME_ERROE,class_name);
			}
			else
			{
				// 如果存在至少一个该类名所对应的对象，则把此类名所对应的所有类对象
				// 的状态都显示出来。

				this->dump_object_of_same_classname(class_name, (*map_iter).second,
					first_param, second_param);
			}
		}
        //清空输出流中的所有信息，以及时显示
        fflush(m_std_out);

		share_memory_manager->set_process_stdout(false);

		ACE_OS::sleep(1);
	}
}

void CObjectMonitorFactory::dump_object_of_same_classname(const char* class_name, 
									const TDumpObjectList& class_object_list, 
									const char* first_param, const char* second_param)
{
	int class_index = 0;

    //是否是帮助命令
    bool b_help = false;
    if( NULL != first_param )
    {
        b_help = (0 == strncmp( first_param, "help", 5 ) );
    }

	TDumpObjectList::const_iterator list_iter = class_object_list.begin();
    //如果是帮助命令，打印帮助信息
    if( b_help )
    {
        if( class_object_list.end() != list_iter )
        {
            FILE_PRINTF("\n");
            (*list_iter)->help_info();
        }
        return ;
    }

    //如果非帮助命令
	for (; list_iter != class_object_list.end(); ++list_iter)
	{
		class_index++;

		/// 打印一行对象状态信息日志的头部信息，以便区分
        FILE_PRINTF("\nClass[%s: %d]: ", class_name, class_index);

        if( 0 != (*list_iter)->dump(first_param, second_param) )
        {
            FILE_PRINTF(IDS_LOG_WCOMMAND_ERROR);


            (*list_iter)->help_info();
            break;
        }
	}
}

void CObjectMonitorFactory::print_all_class_names()
{
	ACE_Guard<ACE_Thread_Mutex> guard(m_objects_mutex);

	int class_count = 0;

	// 循环打印出所有可查看的类名
	TDumpObjectMap::iterator map_iter(this->m_dump_object_map.begin());
	for (; map_iter != m_dump_object_map.end(); ++map_iter)
	{
		//printf("    %02d: %s \n", class_count++, ((*map_iter).first).c_str());
		FILE_PRINTF("    %02d: %s \n", class_count++, ((*map_iter).first).c_str());
	}
}



