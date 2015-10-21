///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           bss_tcp_client.cpp
/// @brief          业务服务器客户端
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月13日
/// @Description
/// @History
///************************************************************
#include "bss_tcp_client.h"

using namespace jsbn;

BssTcpClient::BssTcpClient(unsigned short seq,
                           struct sockaddr_in* sa,
                           short heart_time):PassiveTCPClient(seq, sa, heart_time)
{
}

BssTcpClient::~BssTcpClient()
{
}

bool BssTcpClient::Isvalid()
{
    return _bss_client_info.city_id != ECID_INIT;
}

void BssTcpClient::SetBssClinentInfo(TBssClientInfo info)
{
    _bss_client_info = info;
}

TBssClientInfo& BssTcpClient::GetBssClinentInfo()
{
    return _bss_client_info;
}
