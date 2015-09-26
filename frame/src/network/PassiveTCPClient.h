//
//  PassiveTCPClient.h
//  Frame
//
//  Created by chenjianjun on 15/9/7.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//

#ifndef __PassiveTCPClient_H_
#define __PassiveTCPClient_H_

#include "./NetSignal.h"

namespace NAME_SPACE {
    
    class PassiveTCPClient {
        
    public:
        /**
         *	@brief	构造函数
         *
         *	@param 	sockaddr_in* 客户端地址
         *	@param 	short 心跳事件
         *
         *	@return
         */
        PassiveTCPClient(unsigned short seq, struct sockaddr_in* sa, short heart_time = 10);
        ~PassiveTCPClient();
        
        /**
         *	@brief	启动工作
         *
         *	@param
         *
         *	@return
         */
        bool StartWork(SOCKET, TCPClientSignal*);
        
        /**
         *	@brief	停止工作
         *
         *	@param
         *
         *	@return
         */
        void StopWork();
        
        /**
         *	@brief	发送数据
         *
         *	@paramv const sSendDataPage_ptr& 数据
         *
         *	@return
         */
        int SendData(const sSendDataPage_ptr&);
        
    public:
        
        /**
         *	@brief	接收数据，此函数外部禁止调用，用于event事件回调函数调用
         *
         *	@param 	data 数据
         */
        void PutRecvData(const unsigned char* buf, PacketLength len);

        /**
         *	@brief	事件处理，此函数外部禁止调用，用于event事件回调函数调用
         *
         *	@param 	short 事件集合
         */
        void ProcEvent(EM_NET_EVENT events);
        
    public:
        /*
         连接器类,这个分离很重要,如果不分离会出现小概率崩溃现象,主要是连接函数在调用的时候回调函数里面，
         如果外部删除这个对象会出现内存访问异常，具体看实现（那里是一个自动锁实现,崩溃也在自动锁释放那里）
         */
        TCPClientSignal* _pTCPClientSignal;

    private:
        // 客户端IP
        std::string _client_ip;
        // 客户端端口
        unsigned short _client_port;
        // 心跳时间
        short _heart_time;
        // bufferevent
        struct bufferevent *_bev;
        // 心跳事件器
        struct event *_event;
        // 心跳计数
        unsigned char _heart_num;

        // 连接序号，针对没一个连接使用连接序号，防止出现多个连接共存的时候，一个连接断开，
        // 马上连接上了一个新连接，这个时候系统会把断开的那个连接句柄分配给新上来的连接，
        // 如果我们通过连接句柄异步发送数据，那么有可能出现串包得问题，所以我们使用连接序号
        unsigned short seq_;
    };
    
}

#endif /* defined(__PassiveTCPClient_H_) */
