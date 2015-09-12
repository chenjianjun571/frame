///************************************************************
/// @Copyright (C), 1993-2012, bafei  Information Technologies Co., Ltd.
/// @URL
/// @file           MallocStructFactory.h
/// @brief          内存分配类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2013年01月17日
/// @Description
/// @History
///************************************************************
#ifndef __MALLOC_STRUCT_FACTORY_H_
#define __MALLOC_STRUCT_FACTORY_H_
#include "../base/Common.h"

namespace NAME_SPACE {

    class CMallocStructFactory
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        ///@brief 获取单例
        ///
        ///@param[in] 无
        ///
        ///@return 本类的唯一实例
        //////////////////////////////////////////////////////////////////////////
        static CMallocStructFactory& Instance();

        //////////////////////////////////////////////////////////////////////////
        ///@brief 获取一个发送数据包区
        ///
        ///@param[in] 无
        ///
        ///@return 智能指针
        //////////////////////////////////////////////////////////////////////////
        sSendDataPage_ptr get_send_page();

    private:
        CMallocStructFactory();
        CMallocStructFactory(const CMallocStructFactory &other);
        CMallocStructFactory& operator=(const CMallocStructFactory &other);

        ~CMallocStructFactory();
    };

}

#endif
