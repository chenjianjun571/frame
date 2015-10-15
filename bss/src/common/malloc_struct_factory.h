///************************************************************
/// @Copyright (C), 1993-2012, bafei  Information Technologies Co., Ltd.
/// @URL
/// @file           malloc_struct_factory.h
/// @brief          内存池接口封装
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月14日
/// @Description
/// @History
///************************************************************
#ifndef __MALLOC_STRUCT_FACTORY_H_
#define __MALLOC_STRUCT_FACTORY_H_
#include "module_const_def.h"

class MallocStructFactory
{
public:
    //////////////////////////////////////////////////////////////////////////
    ///@brief 获取单例
    ///
    ///@param[in] 无
    ///
    ///@return 本类的唯一实例
    //////////////////////////////////////////////////////////////////////////
    static MallocStructFactory& Instance();

    //////////////////////////////////////////////////////////////////////////
    ///@brief 获取一个发送数据包区
    ///
    ///@param[in] 无
    ///
    ///@return 智能指针
    //////////////////////////////////////////////////////////////////////////
    sSendDataPage_ptr get_send_page();

private:
    MallocStructFactory();
    MallocStructFactory(const MallocStructFactory &other);
    MallocStructFactory& operator=(const MallocStructFactory &other);

    ~MallocStructFactory();
};

#endif
