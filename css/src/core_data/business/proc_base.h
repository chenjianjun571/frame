///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           proc_base.h
/// @brief          业务处理基类
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月21日
/// @Description
/// @History
///************************************************************
#include "module_const_def.h"
#include "../../communicate/communicate.h"
#include "../../communicate/protoc/protocol_struct.h"

class ProcBase
{
public:
    ProcBase();

    virtual ~ProcBase();

    // 具体处理方法，由派生类实现.
    virtual void Process(const sProtocolData_ptr& msg) = 0;

    // 注册消息处理方法
    static void Register(jsbn::protoc::CommandID, ProcBase*);

    // 执行指定的消息,查询处理方法，调用Process。
    static void Execute(const sProtocolData_ptr& msg);

    // 销毁注册的处理方法
    static void Destroy();

private:
    static std::map<jsbn::protoc::CommandID, ProcBase*> _handers;
};
