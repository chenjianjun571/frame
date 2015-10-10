///************************************************************
/// @Copyright (C), 2015-2030, hzcw  Information Technologies Co., Ltd.
/// @URL
/// @file           CSSClient.cpp
/// @brief          接入服务器监听服务器
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年09月16日
/// @Description
/// @History
///************************************************************
#include "CSSClient.h"
#include "ModuleConfigCollection.h"
#include "../../protoc/ProtocolProcManager.h"
#include "../../../ModuleDataCenter.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace jsbn;
using namespace jsbn::protoc;

CSSClient::CSSClient():_is_run_flg(false)
{}

CSSClient::~CSSClient()
{
}

int CSSClient::Start()
{
    _pActiveTCPClient = new ActiveTCPClient(SYS_CONFIG->get_module_config().css_client_listen_ip,
                                            SYS_CONFIG->get_module_config().css_client_listen_port,
                                            SYS_CONFIG->get_module_config().css_client_heartbeat_detection);
    _pActiveTCPClient->SetTCPClientSignal(this);

    SignalEvent.connect(this, &CSSClient::Event);
    SignalRecvData.connect(this, &CSSClient::RecvData);

    _is_run_flg = true;
    if (!_connect_thread.Start(this))
    {
        _is_run_flg = false;
        delete _pActiveTCPClient;
        return FUNC_FAILED;
    }

    return FUNC_SUCCESS;
}

void CSSClient::Stop()
{
    if (_pActiveTCPClient)
    {
        _is_run_flg = false;
        _connect_thread.Stop();

        SignalEvent.disconnect(this);
        SignalRecvData.disconnect(this);

        _pActiveTCPClient->StopWork();

        delete _pActiveTCPClient;
        _pActiveTCPClient = nullptr;
    }
}

int CSSClient::SendData(const sSendDataPage_ptr& pSend)
{
    if (_pActiveTCPClient)
    {
        return _pActiveTCPClient->SendData(pSend);
    }

    return FUNC_FAILED;
}

void CSSClient::Run(void* arg)
{
    while (_is_run_flg)
    {
        if (!_pActiveTCPClient->IsConnect())
        {
            _pActiveTCPClient->StartWork();
        }

        Thread::SleepMs(2000);
    }
}

void CSSClient::RecvData(unsigned short seq, const unsigned char* buf, PacketLength len)
{
    // 解析数据协议
    sProtocolData_ptr prt = ProtocolProcManager::ParseProtocol(buf, len);
    if (nullptr == prt)
    {
        LOG(ERROR)<<"协议解析失败，关闭连接.";
        _pActiveTCPClient->StopWork();
        return;
    }

    // 标记是那个连接收到的数据，便于业务处理完以后应答
    prt->sock_handle = seq;

    // 判断是否心跳
    if (prt->command_id == Heart_Beat)
    {
        sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
        pSend->sock_handle = prt->sock_handle;
        pSend->Copy(buf, len);
        SendData(pSend);
        return;
    }

    // 丢队列
    ModuleDataCenter::Instance()->PutRecvData(prt);
}

// 套接字事件处理器
void CSSClient::Event(unsigned short seq, EM_NET_EVENT msg)
{
    switch (msg)
    {
        case ENE_HEART:
            LOG(ERROR)<<"需要发送心跳.";
            break;
        case ENE_CONNECTED:
            LOG(INFO)<<"与中心服务器建立连接,ip:"
                    <<SYS_CONFIG->get_module_config().css_client_listen_ip
                   <<"端口:"<<SYS_CONFIG->get_module_config().css_client_listen_port;
            break;
        case ENE_CLOSE:
            LOG(ERROR)<<"与中心服务器断开连接.";
            _pActiveTCPClient->StopWork();
            break;
        case ENE_HEART_TIMEOUT:
            LOG(ERROR)<<"心跳超时,与中心服务器断开连接.";
            _pActiveTCPClient->StopWork();
            break;
        default:
            break;
    }
}
