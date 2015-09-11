//
//  NetFrame.h
//  Frame
//
//  Created by chenjianjun on 15/9/7.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//

#ifndef __Frame__NetFrame__
#define __Frame__NetFrame__

#include "../base/Common.h"
#include "../base/Thread.h"

namespace NAME_SPACE {
    
    class NetFrame {
        
    public:
        static NetFrame* Instance();
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
