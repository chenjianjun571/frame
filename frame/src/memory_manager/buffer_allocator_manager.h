///************************************************************
/// @Copyright (C), 1993-2012, bafei  Information Technologies Co., Ltd.
/// @URL            
/// @file           buffer_allocator_manager.h
/// @brief          管理按大小分配内存的分配器，将内存进行分级管理每一级为一个
///                 内存分配器。当超过所能管理的内存块大小时，交直接从操作系统
///                 分配内存，这一部分内存在释放时，也将直接释放给操作系统
/// @attention      
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2012年12月18日
/// @Description    
/// @History        
///************************************************************

#if !defined (__BUFFERALLOCATORMANAGER_HEADER_FILE__)
#define __BUFFERALLOCATORMANAGER_HEADER_FILE__

#include "./buffer_allocator.h"

/// @class CBufferAllocatorManager
/// @brief 按大小等级管理内存
class CBufferAllocatorManager
{
private:
    /// 将内存分为MAX_GRADE个等级进行管理
    static const size_t MAX_GRADE = 3200;
private:
    /// 最小内存大小
    size_t m_base_size;
    /// 等级增加一级，内存增加的大小
    size_t m_increase_size;
    /// 最大等级的内存大小＝m_base_size + (MAX_GRADE-1)*m_increase_size;
    size_t m_maxsize;

    /// 按大小分配内存的分配器
    CBufferAllocator m_allocators[MAX_GRADE];

public:
    /// @brief 构造函数
    CBufferAllocatorManager (size_t base_size = 32, size_t inc = 32);

    static CBufferAllocatorManager* get_instance ();

    /// @brief 设置管理管理参数
    /// @detail 此函数只应该在没有分配任何内存之前进行调用
    /// @param[in] base_size 最小的内存块的大小
    /// @param[in] inc 相邻等级之间的内存差，即等级增加一级，内存增加的量
    /// @return 0初始化成功，其它初始化失败
    int init (size_t base_size, size_t inc);

    /// @brief 分配大小为s的空间
    /// @param[in] s 待分配的内存大小
    /// @return 分配内存失败，返回NULL，分配内存成功
    void* malloc (size_t s);

    /// @brief 释放内存空间
    /// @param[in] p 待释放的内存空间, 当p为NULL时，不做任何操作
    void free (void *p);

private:
    /// @brief 初始化各分配器
    /// @return 0初始化成功，其它初始化失败
    int init_allocators ();

    /// @brief 分配此内存空间的分配器
    inline size_t get_allocator_index (size_t s);

    inline void* malloc_lager_buffer (size_t s);
    inline void free_lager_buffer (void *p);
};

/// 定义按大小分配内存的分配器
#define BUFFER_ALLOCATOR (CBufferAllocatorManager::get_instance ())

/// 定义按大小分配内存的宏
#define ALLOC_BUFFER(BUFF_SIZE) (BUFFER_ALLOCATOR->malloc (BUFF_SIZE))
/// 定义释放buffer空间的宏
#define FREE_BUFFER(PTR) (BUFFER_ALLOCATOR->free (PTR))

class auto_ptr_buffer
{
private:
    void *m_ptr;
public:
    inline auto_ptr_buffer (void *p) : m_ptr (p) {}

    inline ~auto_ptr_buffer () 
    {
        if (NULL != m_ptr)
        {
            FREE_BUFFER(m_ptr);
            m_ptr = NULL;
        }
    }

    inline void release () { m_ptr = NULL; }
};

#define AUTO_PTR_BUFFER(VAR_NAME, PTR) auto_ptr_buffer VAR_NAME(PTR)

#endif //__BUFFERALLOCATORMANAGER_HEADER_FILE__

