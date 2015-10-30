///************************************************************
/// @Copyright (C), 2015-2030, jsbn  Information Technologies Co., Ltd.
/// @URL
/// @file           proc_ProcRelay.cpp
/// @brief          处理转发
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2015年10月27日
/// @Description
/// @History
///************************************************************
#include "proc_relay.h"
#include "../../../communicate/service/bss/bss_client_manager.h"
#include "../../../communicate/service/cms/cms_client_manager.h"

using namespace jsbn;
using namespace jsbn::protoc;

ProcRelay::ProcRelay()
{}

ProcRelay::~ProcRelay()
{}

void ProcRelay::Process(const sProtocolData_ptr& pMsg)
{
    TDataRelay* pData = (TDataRelay*)pMsg.get();
    sSendDataPage_ptr pSend = MallocStructFactory::Instance().get_send_page();
    pSend->sock_handle = pMsg->sock_handle;
    pSend->Copy(pData->msg, pData->len);

    switch (pData->dst_srv_type)
    {
        case jsbn::protoc::ServiceTpye::ST_BSS:
        {
            if(BssClientManager::Instance().SendData(pData->city_id, pSend) != FUNC_SUCCESS)
            {
                LOG(ERROR)<<"转发数据失败,BSS客户端不在线";
            }

            break;
        }
        case jsbn::protoc::ServiceTpye::ST_CMS:
        {
            if(CmsClientManager::Instance().SendData(pSend) != FUNC_SUCCESS)
            {
                LOG(ERROR)<<"转发数据失败,CMS客户端不在线";
            }

            break;
        }
        case jsbn::protoc::ServiceTpye::ST_CSS:
        {
            LOG(INFO)<<"CSS服务器收到转发协议，消息内容["<<pData->msg<<"]";
            break;
        }
        default:
        {
            LOG(ERROR)<<"无效的转发服务器类型";
            break;
        }
    }
}
