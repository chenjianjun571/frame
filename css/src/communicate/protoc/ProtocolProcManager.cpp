///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           ProtocolProcManager.cpp
/// @brief          协议处理管理类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#include "ProtocolProcManager.h"

using namespace jsbn::protoc;

void delete_recv_page(TProtocolBase* p)
{
    switch(p->command_id)
    {
        case jsbn::protoc::MSG::Heart_Beat:
        {
            CObjectAllocator<TProtocolBase>::get_instance()->free((TProtocolBase*)p);
            break;
        }
        default:
        {
            LOG(ERROR)<<"无法释放此类型的指针,ID:"<<p->command_id;
            break;
        }
    }
}

sProtocolData_ptr ProtocolProcManager::ParseProtocol(const unsigned char* buf, PacketLength len)
{
    // 解析协议，生成一个协议的智能指针区域
    static sNetProtocolDataPage_ptr protocol = std::make_shared<CSSNetProtocol>();

    protocol->Clear();
    if (!protocol->ParseFromArray(buf, len))
    {
        LOG(ERROR)<<"协议解析失败";
        return nullptr;
    }

    return GetRecvData(protocol);
}

sProtocolData_ptr ProtocolProcManager::GetRecvData(sNetProtocolDataPage_ptr& protocol)
{
    sProtocolData_ptr ptr = nullptr;
    switch(protocol->type())
    {
        case jsbn::protoc::MSG::Heart_Beat:
        {
            ptr = sProtocolData_ptr(CObjectAllocator<TProtocolBase>::get_instance()->malloc(), delete_recv_page);

            ptr->command_id = jsbn::protoc::MSG::Heart_Beat;

            break;
        }
        default:
        {
            LOG(ERROR)<<"协议不支持";
            break;
        }
    }

    return ptr;
}
