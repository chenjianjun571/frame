///************************************************************
/// @Copyright (C), 1993-2012, bafei  Information Technologies Co., Ltd.
/// @URL            
/// @file           free_list.h
/// @brief          实现用于进行内存管理的空闲链表
/// @attention      
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2012年12月13日
/// @Description    
/// @History        
///************************************************************

#if !defined (__FREE_LIST_HEADER_FILE__)
#define __FREE_LIST_HEADER_FILE__

#include <cstddef>
#include <cstdio>


/// @class CFessListNode
/// @brief 空闲链表结点结构
class CFreeListNode
{
private:
    /// next 指针
    CFreeListNode *m_next;

public:
    /// @brief 返回可用空间的首地址
    inline void* addr ();

    /// @brief 返回next指针
    inline CFreeListNode* get_next ();

    /// @brief 设置next指针
    inline void set_next (CFreeListNode *_next);
};

class CFreeList
{
private:
    /// 链表大小
    std::size_t m_node_count;

    /// 链表头结点指针
    CFreeListNode *m_pheader;
    
public:
    /// @brief 构造函数
    /// @param[in] nbytes 当前空闲链表维护的内存块的大小
    CFreeList ();

    /// @brief 获取空闲链的大小
    size_t size ()const;

    /// @brief 判断空闲链是否空为
    /// @return true为空
    bool empty ()const;

    /// @brief 从空闲链上移除一块内存块
    /// @detail 只为在空闲链表不为空时，才可以调用此函数
    /// @
    void* remove ();

    /// @brief 向空闲链表中加入新的结点
    /// @param[in] element 新结点的地址, 此指针指向的空间必需大于sizeof(void*)字节
    inline void add_node (CFreeListNode *node);

    inline void dump_list ();
};

inline void CFreeList::dump_list ()
{
    CFreeListNode *p = m_pheader;
    printf ("\n");
    while (NULL != p)
    {
        std::printf ("                             ++++++%p\n", p);
        p = p->get_next ();
    }
}

#include "./free_list.inl"

#endif //__FREE_LIST_HEADER_FILE__

