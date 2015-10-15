//
//  net_frame.h
//  Frame
//
//  Created by chenjianjun on 15/9/7.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//

#ifndef __NET_FRAME__
#define __NET_FRAME__

#include "../base/common.h"
#include "../base/thread.h"

namespace NAME_SPACE {
    
    class NetFrame {
        
    public:
        static NetFrame* Instance();
        static unsigned short GetGloabSeq();
        int NetWorkInit();
        int NetWorkExit();
        
    protected:
        NetFrame();
        ~NetFrame();
        
    private:
        class NetRunnable:public Runnable {
            
        public:
            NetRunnable();
            ~NetRunnable();
            
        protected:
            virtual void Run(void*);
        };
        
        friend class NetRunnable;
        DISALLOW_EVIL_CONSTRUCTORS(NetFrame);
    
    public:
        static struct event_base* _base;
        
    private:
        Thread _main_loop_thread;
        volatile bool _run_flg;
        NetRunnable _runnable;
    };
    
}

#endif /* defined(__Frame__NetFrame__) */
