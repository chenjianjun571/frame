//
//  ActiveTCPClient.h
//  Frame 主动TCP客户端连接类
//
//  Created by chenjianjun on 15/9/8.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//

#ifndef __ActiveTCPClient_H_
#define __ActiveTCPClient_H_

#include <string>
#include "./NetSignal.h"
#include "../base/RWLock.h"

namespace NAME_SPACE {
    
    class ActiveTCPClient {
        
    public:
        explicit ActiveTCPClient(std::string host_name,
                                 unsigned short host_port,
                                 short heart_time = 10);
        
        ~ActiveTCPClient();
        
        void SetTCPClientSignal(TCPClientSignal* pTCPClientSignal) { _pTCPClientSignal = pTCPClientSignal;}
        /**
         *	@brief	启动工作
         *
         *	@param
         *
         *	@return
         */
        bool StartWork();
        
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
         *	@paramv char* 数据
         *	@paramv size_t 数据长度
         *
         *	@return
         */
        int SendData(void* pdata, size_t len);
        
        SOCKET GetFd() { return _fd; }
        void SetHeartFlg(bool flg) { _heart_flg = flg; }
        bool GetHeartFlg() { return _heart_flg; }
        bool IsConnect() { return _connect_flg == 2;}
        
    public:
        /**
         *	@brief	接收数据，此函数外部禁止调用，用于event事件回调函数调用
         *
         *	@param 	void* 数据
         *	@param 	size_t 数据长度
         */
        void PutRecvData(void*, size_t);
        
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
        // 服务器监听地址
        std::string _host_name;
        // 服务器监听端口
        unsigned short _host_port;
        // bufferevent
        struct bufferevent *_bev;
        // 心跳检测时间
        short _heart_time;
        // socket连接句柄
        SOCKET _fd;
        // 心跳事件器
        struct event *_event;
        // 心跳标志
        volatile bool _heart_flg;
        // 心跳计数
        unsigned char _heart_num;
        // 读写锁
        RWLock* _m_rw_loacl;
        // 连接标志 0:未连接 1:连接中 2:已连接
        volatile unsigned char _connect_flg;
    };
    
}

#endif /* defined(__ActiveTCPClient_H_) */
