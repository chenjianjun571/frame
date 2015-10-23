///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           proc_base.cpp
/// @brief          业务处理基类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月21日
/// @Description
/// @History
///************************************************************
#include "proc_base.h"

std::map<jsbn::protoc::CommandID, ProcBase*> ProcBase::_handers;

ProcBase::ProcBase()
{}

ProcBase::~ProcBase()
{}

void ProcBase::Destroy()
{
    for(auto& kvp:_handers)
    {
        delete kvp.second;
        kvp.second = nullptr;
    }

    _handers.clear();
}

//注册消息处理方法
void ProcBase::Register(jsbn::protoc::CommandID msgID, ProcBase* proc)
{
    if (_handers.find(msgID) != _handers.end())
    {
        LOG(ERROR)<<"不能重复注册业务处理handler";
    }
    else
    {
        _handers[msgID] = proc;
    }
}

//执行指定的消息,查询处理方法，调用Process。
void ProcBase::Execute(const sProtocolData_ptr& msg)
{
    auto it = _handers.find(msg->command_id);
    if( it != _handers.end())
    {
        it->second->Process(msg);
    }
    else
    {
        LOG(ERROR) <<"消息["<<msg->command_id<<"]没有对应的处理方法.";
    }
}
