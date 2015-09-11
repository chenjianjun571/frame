#ifndef _SHARE_MEMORY_ALLOCATOR_H_
#define _SHARE_MEMORY_ALLOCATOR_H_

#include <string>

#include "ace/Malloc_T.h"
#include "ace/PI_Malloc.h"
#include "ace/MMAP_Memory_Pool.h"
#include "ace/Process_Mutex.h"
#include "../CodeConfig.h"

_QGGROUP_BEGIN_NESTED_NAMESPACE(frame)

// 后备存储文件大小，默认为10M
#ifdef WIN32
const int BACK_STORE_FILE_SIZE  = 1024 * 1024 * 1;
#else
const int BACK_STORE_FILE_SIZE  = 1024 * 1024 * 10;
#endif


////////////////////////////////////////////////////////////////////
// 
class CShareMemoryAllocator
{
private:
    // 使用内存映射文件，
    typedef ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block> Malloc_Allocator;

public:
    /*************************************************
    Function:       CShareMemoryAllocator
    Description:    默认的构造函数
    Input:          无      
    Output:         无
    Return:         无
    *************************************************/
    CShareMemoryAllocator(void);

    /*************************************************
    Function:       ~CShareMemoryAllocator
    Description:    默认的析构函数
    Input:          无      
    Output:         无
    Return:         无
    *************************************************/
    virtual ~CShareMemoryAllocator(void);

protected:
    // 共享内存分配器
    Malloc_Allocator *m_memory_pool;

    // 共享内存区指针
    void *m_share_memory;

protected:
    // 共享内存后备文件名称
    std::string m_back_store_file;

    // 共享共享内存区大小
    unsigned int m_share_memory_size;

    // 共享内存名称
    std::string m_share_memory_name;

public:
    /*************************************************
    Function:       get_share_memory
    Description:    获得共享内存
    Input:          无      
    Output:         无
    Return:         共享内存基地址
    *************************************************/
    void *get_share_memory(void);

    /*************************************************
    Function:       is_memory_pool_create
    Description:    检查共享内存池是否已创建
    Input:          无      
    Output:         无
    Return:         true    已创建
                    false   未创建
    *************************************************/
    bool is_memory_pool_create(void);

    /*************************************************
    Function:       is_back_store_file_create
    Description:    检查后备存储文件是否已创建
    Input:          无      
    Output:         无
    Return:         true    已创建
                    false   未创建
    *************************************************/
   bool is_back_store_file_create();

   /*************************************************
    Function:       set_back_store_file
    Description:    设置后备存储文件的路径
    Input:          back_store_file    后备存储文件的路径      
    Output:         无
    Return:         无
    *************************************************/
    void set_back_store_file( std::string back_store_file );

    /*************************************************
    Function:       get_back_store_file
    Description:    获得后备存储文件的路径
    Input:          无      
    Output:         无
    Return:         后备存储文件的路径
    *************************************************/
    std::string get_back_store_file(void);

    /*************************************************
    Function:       set_share_memory_size
    Description:    设置共享内存区的大小
    Input:          file_size    共享内存区的大小      
    Output:         无
    Return:         无
    *************************************************/
    void set_share_memory_size( unsigned int memory_size );

    /*************************************************
    Function:       get_share_memory_size
    Description:    获得共享内存区的大小
    Input:          无      
    Output:         无
    Return:         共享内存区的大小
    *************************************************/
    unsigned int get_share_memory_size(void); 

    /*************************************************
    Function:       set_share_memory_name
    Description:    设置共享内存区名称径
    Input:          share_memory_name    共享内存区名称径      
    Output:         无
    Return:         无
    *************************************************/
   void set_share_memory_name( std::string share_memory_name );

   /*************************************************
   Function:       get_share_memory_name
   Description:    获得共享内存区名称径
   Input:          无      
   Output:         无
   Return:         共享内存区名称径
   *************************************************/
   std::string get_share_memory_name(void);

public:
    /*************************************************
    Function:       create_memory_pool
    Description:    创建共享内存池
    Input:          无      
    Output:         无
    Return:         true    成功
                    false   失败
    *************************************************/
    virtual bool create_memory_pool();

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

    /*************************************************
    Function:       release_memory_pool
    Description:    释放共享内存池
    Input:          无      
    Output:         无
    Return:         true    成功    
                    false   失败
    *************************************************/
    virtual bool release_memory_pool();

};

_QGGROUP_END_NESTED_NAMESPACE

#endif

