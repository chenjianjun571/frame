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
    switch(p->command_id) {
    case jsbn::protoc::MSG::Login_Request:
    {
        CObjectAllocator<TLogin>::get_instance()->free((TLogin*)p);
        break;
    }
    }
}

sNetProtocolDataPage_ptr ProtocolProcManager::ParseProtocol(const unsigned char* buf, PacketLength len)
{
    // 解析协议，生成一个协议的智能指针区域
    sNetProtocolDataPage_ptr protocol = std::make_shared<BSSNetProtocol>();

    protocol->Clear();
    if (!protocol->ParseFromArray(buf, len))
    {
        LOG(ERROR)<<"协议解析失败";
        return sNetProtocolDataPage_ptr();
    }


    std::shared_ptr<TProtocolBase> ptr = GetRecvData(protocol->type());
    if(protocol->type() == Login_Request)  {
        ptr->command_id = protocol->type();
        ::memcpy(((TLogin*)ptr.get())->user_name, protocol->loginrequest().username().c_str(), sizeof(((TLogin*)ptr.get())->user_name)-1);
        ::memcpy(((TLogin*)ptr.get())->pass_word, protocol->loginrequest().password().c_str(), sizeof(((TLogin*)ptr.get())->pass_word)-1);
        LOG(ERROR)<<"收到登录应答,用户名:"<<((TLogin*)ptr.get())->user_name<<":密码"<<((TLogin*)ptr.get())->pass_word;
    }

    return protocol;
}

std::shared_ptr<TProtocolBase> ProtocolProcManager::GetRecvData(jsbn::protoc::MSG type) {

    switch(type) {
    case jsbn::protoc::MSG::Login_Request:
    {
        return std::shared_ptr<TProtocolBase>(CObjectAllocator<TLogin>::get_instance()->malloc(), delete_recv_page);
        break;
    }
    }

}
