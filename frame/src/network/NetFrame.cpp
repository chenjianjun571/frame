//
//  NetFrame.cpp
//  Frame
//
//  Created by chenjianjun on 15/9/7.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//
#include "NetFrame.h"
#include <event2/thread.h>

namespace NAME_SPACE {
    
    struct event_base* NetFrame::_base = nullptr;
    
    NetFrame* NetFrame::Instance() {
        LIBJINGLE_DEFINE_STATIC_LOCAL(NetFrame, manager, ());
        return &manager;
    }

    unsigned short NetFrame::GetGloabSeq() {
        static volatile unsigned short seq = 0;
        return seq++;
    }
    
    NetFrame::NetFrame():_run_flg(false){}
    
    NetFrame::~NetFrame(){
        NetWorkExit();
    }
    
    NetFrame::NetRunnable::NetRunnable() {}
    NetFrame::NetRunnable::~NetRunnable() {}
    
    void NetFrame::NetRunnable::Run(void* arg) {
        
        NetFrame* pNetFrame = (NetFrame*)arg;
        while (pNetFrame->_run_flg) {
            Thread::SleepMs(2000);
            event_base_dispatch(NetFrame::_base);
        }
    }
    
    int NetFrame::NetWorkInit() {
        
        if (_run_flg) {
            return FUNC_SUCCESS;
        }
        
        // 开启多线程支持
        evthread_use_pthreads();
        _base = event_base_new();
        evthread_make_base_notifiable(_base);

        // 开启事件监听主线程
        _run_flg = true;
        if (_main_loop_thread.Start(&_runnable, this)) {
            return FUNC_SUCCESS;
        }
        
        // 开始线程失败置运行标志
        _run_flg = false;
        
        return FUNC_FAILED;
    }
    
    int NetFrame::NetWorkExit() {
        
        if (!_run_flg) {
            return FUNC_SUCCESS;
        }
        
        _run_flg = false;
        event_base_loopexit(NetFrame::_base, nullptr);
        _main_loop_thread.Stop();
        event_base_free(_base);
        _base = nullptr;
        
        return FUNC_SUCCESS;
    }
}
