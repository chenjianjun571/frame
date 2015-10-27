///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           proc_bss_register.cpp
/// @brief          处理BSS注册
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月21日
/// @Description
/// @History
///************************************************************
#include "proc_bss_register.h"
#include "../../../communicate/service/bss/bss_client_manager.h"

using namespace jsbn;
using namespace jsbn::protoc;

ProcBssRegister::ProcBssRegister()
{}

ProcBssRegister::~ProcBssRegister()
{}

void ProcBssRegister::Process(const sProtocolData_ptr& pMsg)
{
    TBssClientInfo info;
    info.city_id = ((TRegisterRequest*)pMsg.get())->city_id;

    LOG(INFO)<<"收到注册请求，注册城市id["<<info.city_id<<"]";
    BssClientManager::Instance().SetBssClinentInfo(pMsg->sock_handle, info);

    // 注册应答
    std::string response;
    jsbn::protoc::NetProtocol pc;
    pc.set_commandid(jsbn::protoc::CommandID::Register_Rsp);
    pc.mutable_registerrsp()->set_result(0);
    pc.mutable_registerrsp()->set_error_description("注册成功");
    pc.SerializeToString(&response);

    sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
    pSend->sock_handle = pMsg->sock_handle;
    pSend->Copy(response.c_str(), response.length());

    BssClientManager::Instance().SendData(pSend);
}
