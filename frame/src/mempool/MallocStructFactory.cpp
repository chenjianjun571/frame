/////************************************************************
///// @Copyright (C), 2015-2025, bafei  Information Technologies Co., Ltd.
///// @URL            
///// @file           MallocStructFactory.h
///// @brief          内存分配类
///// @attention
///// @Author         chenjianjun
///// @Version        0.1
///// @Date           2015年08月29日
///// @Description
///// @History
/////************************************************************
//#include "./MallocStructFactory.h"
//#include "../memory_manager/object_allocator.h"
//
//void delete_send_page(stSendMsgPage* p)//一个可执行任意功能的函数
//{
//    CObjectAllocator<stSendMsgPage>::get_instance()->free(p);
//}
//
//CMallocStructFactory& CMallocStructFactory::Instance()
//{
//    static CMallocStructFactory instance;
//
//    return instance;
//}
//
//CMallocStructFactory::CMallocStructFactory()
//{
//}
//
//CMallocStructFactory::~CMallocStructFactory()
//{
//}
//
//sSendMsgPage_ptr CMallocStructFactory::get_send_page()
//{
//    sSendMsgPage_ptr p_str = sSendMsgPage_ptr(CObjectAllocator<stSendMsgPage>::get_instance()->malloc(),delete_send_page);
//
//    return p_str;
//}
//
//sProcMsgPage_ptr CMallocStructFactory::get_mem_by_id(EMessageId id)
//{
//    sProcMsgPage_ptr p_str;
//
////    switch(id)
////    {
////    case CMID_HEART_BEAT_REQ:
////    {
////        p_str = sProcMsgPage_ptr (new(std::nothrow)stHeartBeatReq);
////        break;
////    }
////    case CMID_HEART_BEAT_RESP:
////    {
////        p_str = sProcMsgPage_ptr (new(std::nothrow)stHeartBeatResp);
////        break;
////    }
////    case CMID_APP_LOGIN_SSS_REQ:
////    {
////        p_str= sProcMsgPage_ptr (new(std::nothrow)stAPPLoginSSSReq);
////        break;
////    }
////        //        {
////        //            static object_pool<TIADeployRecord> pl_deploy_record;//对象内存池
//
////        //            sIAProcMsgPage_ptr p_str(pl_deploy_record.construct(),
////        //                                     boost::bind(&object_pool<TIADeployRecord>::destroy, &pl_deploy_record, _1));
//
////        //            return p_str;
////        //        }
////    }
//
//    return p_str;
//}
