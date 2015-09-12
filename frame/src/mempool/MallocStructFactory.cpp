///************************************************************
/// @Copyright (C), 2015-2025, bafei  Information Technologies Co., Ltd.
/// @URL
/// @file           MallocStructFactory.h
/// @brief          内存分配类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年08月29日
/// @Description
/// @History
///************************************************************
#include "./MallocStructFactory.h"
#include "../memory_manager/object_allocator.h"

namespace NAME_SPACE {

    void delete_send_page(stSendMsgPage* p)//一个可执行任意功能的函数
    {
        CObjectAllocator<stSendData>::get_instance()->free(p);
    }

    CMallocStructFactory& CMallocStructFactory::Instance()
    {
        static CMallocStructFactory instance;

        return instance;
    }

    CMallocStructFactory::CMallocStructFactory()
    {
    }

    CMallocStructFactory::~CMallocStructFactory()
    {
    }

    sSendDataPage_ptr CMallocStructFactory::get_send_page()
    {
        return sSendDataPage_ptr(CObjectAllocator<stSendData>::get_instance()->malloc(),delete_send_page);
    }

}
