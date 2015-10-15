#include <ace/OS.h>

#include "./share_memory_allocator.h"

namespace NAME_SPACE
{
    namespace frame
    {
        using std::string;

        /*************************************************
        Function:       CShareMemoryManager
        Description:    默认的构造函数
        Input:          无
        Output:         无
        Return:         无
        *************************************************/
        CShareMemoryAllocator::CShareMemoryAllocator(void) :
        m_memory_pool(NULL), m_share_memory(NULL),
        m_share_memory_size(BACK_STORE_FILE_SIZE)
        {
            //
        }

        /*************************************************
        Function:       ~CShareMemoryManager
        Description:    默认的析构函数
        Input:          无
        Output:         无
        Return:         无
        *************************************************/
        CShareMemoryAllocator::~CShareMemoryAllocator(void)
        {
            //
            clear_memory_pool();
            release_memory_pool();
            m_memory_pool = NULL;
            m_share_memory = NULL;
        }

        /*************************************************
        Function:       get_share_memory
        Description:    获得共享内存
        Input:          无
        Output:         无
        Return:         共享内存基地址
        *************************************************/
        void *CShareMemoryAllocator::get_share_memory(void)
        {
            return this->m_share_memory;
         }

        /*************************************************
        Function:       is_memory_pool_create
        Description:    检查共享内存池是否已创建
        Input:          无
        Output:         无
        Return:         true    已创建
        false   未创建
        *************************************************/
        bool CShareMemoryAllocator::is_memory_pool_create(void)
        {
            if ( this->m_memory_pool == NULL )
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        /*************************************************
        Function:       is_back_store_file_create
        Description:    检查后备存储文件是否已创建
        Input:          无
        Output:         无
        Return:         true    已创建
        false   未创建
        *************************************************/
        bool CShareMemoryAllocator::is_back_store_file_create()
        {
            if ( ACE_OS::filesize( this->m_back_store_file.c_str() ) == -1 )
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        /*************************************************
        Function:       set_back_store_file
        Description:    设置后备存储文件的路径
        Input:          back_store_file    后备存储文件的路径
        Output:         无
        Return:         无
        *************************************************/
        void CShareMemoryAllocator::set_back_store_file( string back_store_file )
        {
            this->m_back_store_file = back_store_file;
        }

        /*************************************************
        Function:       get_back_store_file
        Description:    获得后备存储文件的路径
        Input:          无
        Output:         无
        Return:         后备存储文件的路径
        *************************************************/
        string CShareMemoryAllocator::get_back_store_file(void)
        {
            return this->m_back_store_file;
        }

        /*************************************************
        Function:       set_share_memory_size
        Description:    设置共享内存区的大小
        Input:          memory_size    共享内存区的大小
        Output:         无
        Return:         无
        *************************************************/
        void CShareMemoryAllocator::set_share_memory_size( unsigned int memory_size )
        {
            this->m_share_memory_size = memory_size;
        }

        /*************************************************
        Function:       get_share_memory_size
        Description:    获得共享内存区的大小
        Input:          无
        Output:         无
        Return:         共享内存区的大小
        *************************************************/
        unsigned int CShareMemoryAllocator::get_share_memory_size(void)
        {
            return this->m_share_memory_size;
        }

        /*************************************************
        Function:       set_share_memory_name
        Description:    设置共享内存区名称径
        Input:          share_memory_name    共享内存区名称径
        Output:         无
        Return:         无
        *************************************************/
        void CShareMemoryAllocator::set_share_memory_name( string share_memory_name )
        {
            this->m_share_memory_name = share_memory_name;
        }

        /*************************************************
        Function:       get_share_memory_name
        Description:    获得共享内存区名称径
        Input:          无
        Output:         无
        Return:         共享内存区名称径
        *************************************************/
        string CShareMemoryAllocator::get_share_memory_name(void)
        {
            return this->m_share_memory_name;
        }

        /*************************************************
        Function:       create_memory_pool
        Description:    创建共享内存池
        Input:          无
        Output:         无
        Return:         true    成功
        false   失败
        *************************************************/
        bool CShareMemoryAllocator::create_memory_pool()
        {
            //// 加区域锁
            //Guard guard(CSharedMemoryManager::lock);


            if ( this->m_memory_pool != NULL )
            {
                return true;
            }

            // 设置内存池，基本配置
            ACE_MMAP_Memory_Pool_Options options( ACE_DEFAULT_BASE_ADDR, ACE_MMAP_Memory_Pool_Options::ALWAYS_FIXED,0,this->m_share_memory_size );

            // 创建 共享内存池
            ACE_NEW_RETURN( this->m_memory_pool, Malloc_Allocator( this->m_back_store_file.c_str(), this->m_back_store_file.c_str(), &options ),false );

            // 修改后备存储文件的访问权限。
            //chmod( this->m_back_store_file.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | options.file_mode_ );

            return true;
        }

        /*************************************************
        Function:       init_memory_pool
        Description:    初始化共享内存池
        Input:          无
        Output:         无
        Return:         true    成功
        false   失败
        *************************************************/
        bool CShareMemoryAllocator::init_memory_pool()
        {
            //// 加区域锁
            //Guard guard(CSharedMemoryManager::lock);

            // 检查内存池是否已创建
            if ( this->m_memory_pool == NULL )
            {
                return false;
            }

            //////////////////////////////////////////////////////////////////////////
            // 1、检查内存池是否已初始化
            if ( this->m_memory_pool->find( this->m_share_memory_name.c_str(), (void *&) this->m_share_memory ) == 0 )
            {
                // 当 find 成功时，如果没有返回正确的内存区地址,
                if ( this->m_share_memory == NULL )
                {
                    // 没有找到正确的内存区地址
                    return false;
                }
                else
                {
                    return true;
                }
            }

            //////////////////////////////////////////////////////////////////////////
            // 2、初始化内存池
            try
            {
                // 分配共享内存空间，并初始化值为空
                this->m_share_memory = this->m_memory_pool->calloc( this->m_share_memory_size, '\0' );

                if ( this->m_share_memory == NULL )
                {
                    // 分配共享内存失败
                    return false;
                }

                // 在系统中注册该共享内存区，以共享内存区名称作为标识
                if (this->m_memory_pool->bind( this->m_share_memory_name.c_str(), this->m_share_memory ) != 0)
                {
                    clear_memory_pool();
                    return false;
                }

                return true;
            }
            catch( ... )
            {
                return false;
            }

            return true;
        }

        /*************************************************
        Function:       clear_memory_pool
        Description:    清除共享内存池
        Input:          无
        Output:         无
        Return:         true    成功
        false   失败
        *************************************************/
        bool CShareMemoryAllocator::clear_memory_pool()
        {
            // 当运行标志为空时，退出本次操作
            if ( this->m_share_memory == NULL )
            {
                return true;
            }

            if (this->m_memory_pool == NULL)
            {
                return true;
            }

            try
            {
                // 从系统中注销该共享内存区
                this->m_memory_pool->unbind( this->m_share_memory_name.c_str() );

                // 释放共享内存
                this->m_memory_pool->free( this->m_share_memory );

                this->m_share_memory = NULL;

                // 释放资源
                this->m_memory_pool->remove();

                return true;
            }
            catch( ... )
            {
                return false;
            }
        }


        /*************************************************
        Function:       release_memory_pool
        Description:    释放共享内存池
        Input:          无
        Output:         无
        Return:         true    成功
        false   失败
        *************************************************/
        bool CShareMemoryAllocator::release_memory_pool()
        {
            //// 加区域锁
            //Guard guard(CSharedMemoryManager::lock);

            // 当运行标志为空时，退出本次操作
            if ( this->m_memory_pool == NULL )
            {
                return true;
            }

            try
            {
                // 释放内存池
                delete this->m_memory_pool;

                this->m_memory_pool = NULL;

               return true;
            }
            catch( ... )
            {
                return false;
            }
        }
    }
}
