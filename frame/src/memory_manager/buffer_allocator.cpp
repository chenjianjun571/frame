#include "./buffer_allocator.h"
#include <cstdlib>
#include <cstdio>
#include <new>
#include <ace/Guard_T.h>

using namespace std;


CBufferAllocator::CBufferAllocator (size_t hwm, size_t fhwm, size_t inc)
{
    init (hwm, fhwm, inc);

    m_chunk_size = (sizeof(SChunk) > sizeof(void*) ? sizeof(SChunk) : sizeof(void*));
    m_chunk_cnt = 0;
}

inline void* CBufferAllocator::malloc_chunk ()
{
    return ::malloc (m_chunk_size);
}

inline void CBufferAllocator::free_chunk (void *p)
{
    ::free (p);
}

void* CBufferAllocator::malloc ()
{
    void *p = NULL;
    {
        ACE_Guard<ACE_Thread_Mutex> guard(m_freelist_mutex);
        // 如果已经没有空闲内存块了, 向操作系统申请一批
        if (m_freelist.empty ())
        {
            // 计算还可以申请的内存量
            size_t n = (m_hwm - m_chunk_cnt);
            // 计算本次的申请量
            n = (n > m_each_inc ? m_each_inc : n);

            if (n < 1) return NULL;

            // 向操作系统申请n个内存块
            if (remalloc (n) < 1)
            {
                // 如果没有申请到内存
                return NULL;
            }
//            m_freelist.dump_list ();
        }

        // 从空闲链上取一个内存块
        p = m_freelist.remove ();
    }

    // 给内存块打上标记, 以检查内存是否存在写越界
    SChunk *pchunk = new (p) SChunk(this);


    //dump_info ();
    
    // 计算计算可用的内存地址,并返回
    char *pchar = (char*)p;
    void *pvoid = (void*)(pchar + sizeof(SChunk));
    return pvoid;
}

void CBufferAllocator::free (void *p)
{
    ACE_Guard<ACE_Thread_Mutex> guard(m_freelist_mutex);
    if (m_freelist.size () > m_fhwm)
    {
        free_chunk (p);
    }

    CFreeListNode *pnode = (new(p) CFreeListNode);
    // 添加到空闲链
    m_freelist.add_node (pnode);

    //dump_info ();
}

size_t CBufferAllocator::remalloc (size_t cnt)
{
    size_t acnt = 0;
    // 向操作系统申请内存
    for (; acnt < cnt; ++acnt)
    {
        void *p = malloc_chunk ();
        if (NULL == p) break;

        // 申请内存成功，构造空闲链结点
        CFreeListNode *pnode = (new(p) CFreeListNode);
        
        // 添加到空闲链
        m_freelist.add_node (pnode);

    }

    m_chunk_cnt += acnt;
    
    return acnt;
}

void CBufferAllocator::dump_info ()const
{
    printf ("chunk size: %-5d malloc cnt:%-5d  free cnt:%-5d\n", 
            m_chunk_size, m_chunk_cnt, m_freelist.size ());
}

