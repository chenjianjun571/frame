///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           ProtocolProcManager.h
/// @brief          协议处理管理类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#ifndef __PROTOCOL_PROC_MANAGER_H_
#define __PROTOCOL_PROC_MANAGER_H_

#include "ModuleConstDef.h"
#include "ProtocolStruct.h"
#include "../Communicate.h"

template<T>
void delete_recv_page(T* p)
{
    CObjectAllocator<T>::get_instance()->free(p);
}

class ProtocolProcManager
{
public:
    // 解析通信协议
    static sNetProtocolDataPage_ptr ParseProtocol(const unsigned char*, PacketLength);

    template<T>
    std::shared_ptr<TProtocolBase> GetRecvData(jsbn::protoc::MSG type) {

        switch(type) {
        case jsbn::protoc::MSG::Login_Request:
        {
            return std::shared_ptr<TProtocolBase>(CObjectAllocator<T>::get_instance()->malloc(), delete_recv_page);
            break;
        }
        }

    }

};

#endif //__PROTOCOL_PROC_MANAGER_H_

