///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
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
    _city_id = ECID_INIT;
}

BssTcpClient::~BssTcpClient()
{
}

void BssTcpClient::SetCityID(EM_CITY_ID id)
{
    _city_id = id;
}

EM_CITY_ID BssTcpClient::GetCityID()
{
    return _city_id;
}
