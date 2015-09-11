///************************************************************
/// @Copyright (C), 1993-2012, bafei  Information Technologies Co., Ltd.
/// @URL            
/// @file           buffer_allocator.h
/// @brief          按固定大小进行管理的分配器，一个分配器只能管理一种大小的内存块
///                 将可用的内存，使用空间单向链表进行管理，当分配出去时，在每一个
///                 分配出现的内存块前加上一个chunk头，用于标记内存块的大小以及数据
///                 校验，数据校验的目的在于，检查是否存在写越界
/// @attention      
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2012年12月14日
/// @Description    
/// @History        
///************************************************************

#if !defined (__BUFFER_ALLOCATOR_HEADER_FILE__)
#define __BUFFER_ALLOCATOR_HEADER_FILE__

#include "./free_list.h"
#include <ace/Thread_Mutex.h>

class CBufferAllocator;

/// 可向操作系统分配的内存块的默认最大值
const std::size_t MM_DEFAULT_FREELIST_HWM = 0xFFFFFFFF;
/// 空闲链表的值,即内存管理器维护的空闲内存块的最大值
const std::size_t MM_DEFAULT_FREELIST_FHMM = 200;
/// 每次向操作系统申请的内存块的个数
const std::size_t MM_DEFAULT_FREELIST_INC = 5;

#pragma pack(push,1)
/// @class SChunk 
/// @brief 进行内存管理的chunk
struct SChunk
{
private:
    static const unsigned char VERIFY_VALUE = 0xAD;
    /// 用于进行内存校验
    unsigned char verify_byte1;
    /// 内存块所属的空闲链
    CBufferAllocator *owner;
    unsigned char verify_byte2;
public:
    /// @brief 构造函数
    inline SChunk (CBufferAllocator *_owner = NULL);

    /// @brief 对内存块进行维护标记其校验位，以及内存所属的空闲链
    /// @param[in] _owner 内存块所属的空闲链
    inline void mask (CBufferAllocator *_owner);

    /// @brief 检查检查是否有效，有效是指是由内存管理器分配的
    /// @return true 有效，false无效
    inline bool verify_valid ();

    /// @brief 获取当前chunk属的管理者
    inline CBufferAllocator* get_owner () {return owner;}
};
#pragma pack (pop)

/// @class CBufferAllocator
/// @brief 按大小分配内存管理
class CBufferAllocator
{
private:
    /// 内存块的大小
    size_t m_chunk_size;
    /// 从操作系统申请的chunk的个数
    size_t m_chunk_cnt;

    /// High Water Mark 允许分配的内存块的大小个数
    size_t m_hwm;
    /// Free High Water Mark 允许拥有的空闲块的最大个数
    size_t m_fhwm;
    /// Each Increment value 内存不够时，向操作系统批量申请的内存块个数
    size_t m_each_inc;

    /// 空闲链表
    CFreeList m_freelist;
    /// 互斥锁
    ACE_Thread_Mutex m_freelist_mutex;
public:
    /// @brief 构造函数
    CBufferAllocator (std::size_t hwm = MM_DEFAULT_FREELIST_HWM ,
            std::size_t fhmm = MM_DEFAULT_FREELIST_FHMM,
            size_t inc = MM_DEFAULT_FREELIST_INC);

    /// @brief 设置内存块的大小
    /// @detail 在使用此类对象分配内存前，调用此函数，一旦分配内存后
    ///     请不要再调用此函数，修改内存块大小，此将导致维护的内存块大小不一
    /// @param[in] nbytes 可供使用的buffer大小，单位字节
    inline void set_buffer_size (size_t nbytes);

    /// 
    /// @brief 设置内存管理参数信息
    /// @param[in] hwn 向操作系统申请的内存块的最大个数, 当达到此个数时，
    ///         不再向操作系统申请内存
    /// @param[in] fhwn 内存管理器维护的最大空闲块个数，当超过此值时，会
    ///         将释放的内存归还给操作系统
    /// @param[in] inc 每一次向操作系统申请的内存块个数
    /// @author lijing
    /// @date 2012年12月14日 星期五 11时07分56秒
    inline void init (size_t hwn, size_t fhwn, size_t inc);

    /// @brief 申请内存
    /// @return NULL 分配内存失败，分配内存成功，返回可用内存的首地址
    void* malloc ();

    /// @brief 释放内存空间
    /// @detail 此将内存真的还给操作系统，或者加入空闲链表
    /// @param[in] p 待释放的内存空间
    void free (void *p);

    /// @brief 打印出信息
    void dump_info ()const;

private:
    /// @brief 分配内存块
    inline void* malloc_chunk ();

    /// @brief 释放内存块
    inline void free_chunk (void *p);

    /// @brief 再向操作系统申请cnt个内存块，加入空闲链表
    /// @param[in] cnt待分配的内存块个数
    /// @return 本次从操作系统成功申请的内存块个数
    size_t remalloc (size_t cnt);
};

inline void SChunk::mask (CBufferAllocator *_owner)
{
    verify_byte1 = VERIFY_VALUE;
    owner = _owner;
    verify_byte2 = VERIFY_VALUE;

//    printf ("%X  %p  %X\n", verify_byte1, owner, verify_byte2);
}

inline SChunk::SChunk (CBufferAllocator *_owner)
{
    mask (_owner);
}

inline bool SChunk::verify_valid ()
{
    return (VERIFY_VALUE == verify_byte1 && VERIFY_VALUE == verify_byte2);
}

inline void CBufferAllocator::set_buffer_size (size_t nbytes)
{
    size_t chunk_size = nbytes + sizeof(SChunk);
    m_chunk_size >= chunk_size ? m_chunk_size : m_chunk_size = chunk_size; 
}

inline void CBufferAllocator::init (size_t hwm, size_t fhwm, size_t inc)
{
    m_hwm = hwm;
    m_fhwm = fhwm;
    m_each_inc = inc;
}

#endif //__BUFFER_ALLOCATOR_HEADER_FILE__

