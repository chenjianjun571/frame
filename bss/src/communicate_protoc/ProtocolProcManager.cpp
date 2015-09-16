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

sNetProtocolDataPage_ptr ProtocolProcManager::ParseProtocol(jsbn::sRecvDataPage_ptr& data)
{
    // 解析协议，生成一个协议的智能指针区域
    static sNetProtocolDataPage_ptr protocol(new(std::nothrow) jsbn::protoc::BSSNetProtocol());

    protocol->Clear();
    if (!protocol->ParseFromArray(data->recv_buf, data->recv_len)) {
        LOG(ERROR)<<"协议解析";
    }

    return protocol;
}
