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

void delete_recv_bc_page(TBCProtocolBase* p)
{
    switch(p->command_id)
    {
        case jsbn::protoc::BC_MSG::EBC_Heart_Beat:
        {
            CObjectAllocator<TBCProtocolBase>::get_instance()->free((TBCProtocolBase*)p);
            break;
        }
        default:
        {
            LOG(ERROR)<<"无法释放此类型的指针,ID:"<<p->command_id;
            break;
        }
    }
}

void delete_recv_sc_page(TSCProtocolBase* p)
{
    switch(p->command_id)
    {
        case jsbn::protoc::SC_MSG::ESC_Heart_Beat:
        {
            CObjectAllocator<TSCProtocolBase>::get_instance()->free((TSCProtocolBase*)p);
            break;
        }
        default:
        {
            LOG(ERROR)<<"无法释放此类型的指针,ID:"<<p->command_id;
            break;
        }
    }
}

sBCProtocolData_ptr ProtocolProcManager::ParseBCProtocol(const unsigned char* buf, PacketLength len)
{
    // 解析协议，生成一个协议的智能指针区域
    static sBCNetProtocolDataPage_ptr protocol = std::make_shared<BCNetProtocol>();

    protocol->Clear();
    if (!protocol->ParseFromArray(buf, len))
    {
        LOG(ERROR)<<"协议解析失败";
        return nullptr;
    }

    sBCProtocolData_ptr ptr = nullptr;
    switch(protocol->type())
    {
        case jsbn::protoc::BC_MSG::EBC_Heart_Beat:
        {
            ptr = sBCProtocolData_ptr(CObjectAllocator<TBCProtocolBase>::get_instance()->malloc(), delete_recv_bc_page);

            ptr->command_id = jsbn::protoc::BC_MSG::EBC_Heart_Beat;

            break;
        }
        default:
        {
            LOG(ERROR)<<"BSS与CSS之前的通信协议不支持";
            break;
        }
    }

    return ptr;
}

sSCProtocolData_ptr ProtocolProcManager::ParseSCProtocol(const unsigned char* buf, PacketLength len)
{
    // 解析协议，生成一个协议的智能指针区域
    static sSCNetProtocolDataPage_ptr protocol = std::make_shared<SCNetProtocol>();

    protocol->Clear();
    if (!protocol->ParseFromArray(buf, len))
    {
        LOG(ERROR)<<"协议解析失败";
        return nullptr;
    }

    sSCProtocolData_ptr ptr = nullptr;
    switch(protocol->type())
    {
        case jsbn::protoc::SC_MSG::ESC_Heart_Beat:
        {
            ptr = sSCProtocolData_ptr(CObjectAllocator<TSCProtocolBase>::get_instance()->malloc(), delete_recv_sc_page);

            ptr->command_id = jsbn::protoc::SC_MSG::ESC_Heart_Beat;

            break;
        }
        default:
        {
            LOG(ERROR)<<"SMS与CSS之前的通信协议不支持";
            break;
        }
    }

    return ptr;
}
