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
#include "./malloc_struct_factory.h"

using namespace jsbn;

void delete_send_page(stSendData* p)
{
    CObjectAllocator<stSendData>::get_instance()->free(p);
}

MallocStructFactory& MallocStructFactory::Instance()
{
    static MallocStructFactory instance;
    return instance;
}

MallocStructFactory::MallocStructFactory() {}

MallocStructFactory::~MallocStructFactory() {}

sSendDataPage_ptr MallocStructFactory::get_send_page()
{
    return sSendDataPage_ptr(CObjectAllocator<stSendData>::get_instance()->malloc(), delete_send_page);
}
