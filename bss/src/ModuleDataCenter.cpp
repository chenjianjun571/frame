///************************************************************
/// @Copyright (C), 1993-2022, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           ModuleDataCenter.h
/// @brief          数据存取中心
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#include "ModuleDataCenter.h"

ModuleDataCenter* ModuleDataCenter::Instance() {
    static ModuleDataCenter mInstance;
    return &mInstance;
}

ModuleDataCenter::ModuleDataCenter():_recv_cond_variable(jsbn::ConditionVariable::Create())
{}

ModuleDataCenter::~ModuleDataCenter()
{
    _recv_data_lists.clear();
}

int ModuleDataCenter::PutRecvData(jsbn::sRecvDataPage_ptr& pdata)
{
    jsbn::CriticalSectionScoped css(&_recv_critical_section);

    // 判断数据积压是否达到上限
    if (_recv_data_lists.size() > 10000) {
        LOG(INFO)<<"接收数据队列发生积压，做丢弃处理.";
        return FUNC_FAILED;
    }

    _recv_data_lists.push_back(pdata);

    // 通知数据请求线程有数据到达
    _recv_cond_variable->WakeAll();

    return FUNC_SUCCESS;
}

jsbn::sRecvDataPage_ptr ModuleDataCenter::GetRecvData()
{
    jsbn::CriticalSectionScoped css(&_recv_critical_section);

    // 如果没有数据就等待数据到达
    if (_recv_data_lists.size() == 0) {
        _recv_cond_variable->SleepCS(_recv_critical_section, 1000);
    }

    // 延迟一秒是否有数据到达，有的话取出返回，没有的话返回一个空智能指针
    if (_recv_data_lists.size() == 0) {
        return jsbn::sRecvDataPage_ptr();
    }

    jsbn::sRecvDataPage_ptr pdata = _recv_data_lists.front();
    _recv_data_lists.pop_front();

    return pdata;
}
