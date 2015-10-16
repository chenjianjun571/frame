///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           protocol_proc_manager.cpp
/// @brief          协议处理管理类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#include "protocol_proc_manager.h"

void delete_recv_bc_page(TBCProtocolBase* p)
{
    switch(p->command_id)
    {
        case jsbn::protoc::bc::CommandID::Heart_Beat:
        {
            CObjectAllocator<TBCProtocolBase>::get_instance()->free((TBCProtocolBase*)p);
            break;
        }
        case jsbn::protoc::bc::CommandID::Register_Request:
        {
            CObjectAllocator<TBCRegisterRequest>::get_instance()->free((TBCRegisterRequest*)p);
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
    static sBCNetProtocolDataPage_ptr protocol = std::make_shared<jsbn::protoc::bc::NetProtocol>();

    protocol->Clear();
    if (!protocol->ParseFromArray(buf, len))
    {
        LOG(ERROR)<<"协议解析失败";
        return nullptr;
    }

    sBCProtocolData_ptr ptr = nullptr;
    switch(protocol->commandid())
    {
        case jsbn::protoc::bc::CommandID::Heart_Beat:
        {
            ptr = sBCProtocolData_ptr(CObjectAllocator<TBCProtocolBase>::get_instance()->malloc(), delete_recv_bc_page);

            ptr->command_id = jsbn::protoc::bc::CommandID::Heart_Beat;

            break;
        }
        case jsbn::protoc::bc::CommandID::Register_Request:
        {
            ptr = sBCProtocolData_ptr(CObjectAllocator<TBCRegisterRequest>::get_instance()->malloc(), delete_recv_bc_page);

            ptr->command_id = jsbn::protoc::bc::CommandID::Register_Request;

            ((TBCRegisterRequest*)ptr.get())->city_id = protocol->registerrequest().cityid();

            LOG(INFO)<<"收到业务服务器注册请求,城市ID:"<< ((TBCRegisterRequest*)ptr.get())->city_id;

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
