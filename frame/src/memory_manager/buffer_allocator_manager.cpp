#include "./buffer_allocator_manager.h"
#include <new>

using namespace std;

CBufferAllocatorManager::CBufferAllocatorManager (size_t base_size, size_t inc)
{
    init (base_size, inc);
}

CBufferAllocatorManager* CBufferAllocatorManager::get_instance ()
{
    static CBufferAllocatorManager *_instance = NULL;
    if (NULL == _instance)
    {
        if (NULL == _instance)
        {
            _instance = new(nothrow) CBufferAllocatorManager ();
        }
    }
    return _instance;
}

int CBufferAllocatorManager::init (size_t base_size, size_t inc)
{
    m_base_size = base_size;
    m_increase_size = inc;

    // 分配器所能分配的最大内存
    m_maxsize = m_base_size + (MAX_GRADE-1) * m_increase_size;
    
    return init_allocators ();
}

int CBufferAllocatorManager::init_allocators ()
{
    for (size_t i = 0; i < MAX_GRADE; ++i)
    {
        size_t s = m_base_size + i * m_increase_size;

        m_allocators[i].set_buffer_size (s);
    }
    return 0;

}

inline size_t CBufferAllocatorManager::get_allocator_index (size_t s)
{
    if (s <= m_base_size) 
    {
        return 0;
    }
    else if (s <= m_maxsize)
    {
        return (s + m_increase_size - 1 - m_base_size) / m_increase_size;
    }
    return MAX_GRADE;
}

inline void* CBufferAllocatorManager::malloc_lager_buffer (size_t s)
{
    size_t alloc_s = s + sizeof (SChunk);
    
    char *pchar = (char*)::malloc (alloc_s);
    if (NULL == pchar) return NULL;

    SChunk *pchunk = new(pchar)SChunk (NULL);

    return ((void*)(pchar + sizeof(SChunk)));
}

inline void CBufferAllocatorManager::free_lager_buffer (void *p)
{
    ::free (p);
}

void* CBufferAllocatorManager::malloc (size_t s)
{
    if (0 == s) return NULL;

    size_t aindex = get_allocator_index (s);
    if (MAX_GRADE != aindex)
    {
        return m_allocators[aindex].malloc ();
    }

    return malloc_lager_buffer (s);
}

void CBufferAllocatorManager::free (void *p)
{
    if (NULL == p) return;

    SChunk *pchunk = (SChunk*)( ((char*)p) - sizeof(SChunk));
    // 校验数据是否被篡改
    if (!pchunk->verify_valid ())
    {
        printf ("ERROR！！！要释放的内存不是通过内存管理模块分配的，或者存在写溢出.\n");
        return ;
    }
    CBufferAllocator *pallocator = pchunk->get_owner ();
    if (NULL != pallocator)
    {
        pallocator->free (pchunk);
    }
    else
    {
        free_lager_buffer (pchunk);
    }
}


