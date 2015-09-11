///************************************************************
/// @Copyright (C), 1993-2012, bafei  Information Technologies Co., Ltd.
/// @URL            
/// @file           object_allocator.h
/// @brief          实现按类型管理内存
/// @attention      
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2012年12月18日 星期二 17时13分41秒
/// @Description    
/// @History        
///************************************************************

#if !defined (__BOBJECT_ALLOCATOR_FILE_HEADER__)
#define __BOBJECT_ALLOCATOR_FILE_HEADER__

/// @class CObjectAllocator
/// @brief 按类型分配对象

template<class T>
class CObjectAllocator
{
private:
    CObjectAllocator (){}
public:
    static CObjectAllocator* get_instance ();

    /// @brief 分配内存，分配成功，返回所配置的内存块的地址
    T* malloc ();
    
    /// @brief 释放内存，释放内存管理
    void free (T* p);
};

#define OBJECT_ALLOCATOR(TYPE) (CObjectAllocator<TYPE>::get_instance ())

#define ALLOC_OBJECT(TYPE) (OBJECT_ALLOCATOR(TYPE)->malloc ())

#define FREE_OBJECT(TYPE, PTR) (OBJECT_ALLOCATOR(TYPE)->free (PTR))

template<class T>
class auto_ptr_obj
{
private:
    T *m_ptr;
public:
    inline auto_ptr_obj (T *p) : m_ptr (p) {}

    inline ~auto_ptr_obj () 
    {
        if (NULL != m_ptr)
        {
            FREE_OBJECT(T, m_ptr);
            m_ptr = NULL;
        }
    }

    inline void release () { m_ptr = NULL; }
};

#define AUTO_PTR_OBJ(TYPE, VAR_NAME, PTR) auto_ptr_obj<TYPE> VAR_NAME(PTR)

#include "./object_allocator.inl"

#endif //__BOBJECT_ALLOCATOR_FILE_HEADER__

