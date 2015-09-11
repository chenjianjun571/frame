#include "./buffer_allocator_manager.h"

template<class T>
CObjectAllocator<T>* CObjectAllocator<T>::get_instance ()
{
    static CObjectAllocator* _instance = NULL;
    if (NULL == _instance)
    {
        if (NULL == _instance)
        {
            _instance = new(std::nothrow) CObjectAllocator;
        }
    }
    return _instance;
}

template<class T>
T* CObjectAllocator<T>::malloc ()
{
    void *pvoid  = ALLOC_BUFFER (sizeof(T));
    T *ptr = NULL;
    if (NULL != pvoid)
    {
        ptr = new(pvoid) T();
    }
    return ptr;
}

template<class T>
void CObjectAllocator<T>::free (T* p)
{
    if (NULL != p)
    {
        p->T::~T();
        FREE_BUFFER (p);
    }
}

