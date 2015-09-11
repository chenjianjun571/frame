
inline void* CFreeListNode::addr ()
{
    return (void*)this;
}

inline CFreeListNode* CFreeListNode::get_next ()
{
    return m_next;
}

inline void CFreeListNode::set_next (CFreeListNode *_next)
{
    m_next = _next;
}

inline CFreeList::CFreeList ()
{
    m_node_count = 0;
    m_pheader = NULL;
}

inline size_t CFreeList::size ()const
{
    return m_node_count;
}

inline bool CFreeList::empty ()const
{
    return (NULL == m_pheader);
}

inline void* CFreeList::remove ()
{
    CFreeListNode *p = m_pheader;

    m_pheader = p->get_next();
    --m_node_count;
    
    return p->addr();
}

inline void CFreeList::add_node (CFreeListNode *node)
{
    node->set_next (m_pheader);
    m_pheader = node;
    ++m_node_count;
}

