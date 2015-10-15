//
//  net_signal.h
//  Frame
//
//  Created by chenjianjun on 15/9/8.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//

#ifndef __NET_SIGNAL_H_
#define __NET_SIGNAL_H_

#include "../base/sigslot.h"
#include "../base/common.h"

namespace NAME_SPACE {
    
    class TCPServerSignal {
        
    public:
        TCPServerSignal() {}
        ~TCPServerSignal() {}
        
        // 客户端连接触发器
        sigslot::signal2<SOCKET , struct sockaddr_in*> SignalAccept;
    };
    
    class TCPClientSignal {
        
    public:
        TCPClientSignal() {}
        ~TCPClientSignal() {}
        
        // 数据接收连接器
        sigslot::signal3<unsigned short, const unsigned char*, PacketLength> SignalRecvData;
        // 套接字事件处理器
        sigslot::signal2<unsigned short, EM_NET_EVENT> SignalEvent;
    };
    
}

#endif /* defined(__NetSignal_H_) */
