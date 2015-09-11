//
//  ServerWorker.h
//  服务器对象类
//
//  Created by chenjianjun on 15/9/7.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//

#ifndef __ServerWorker_H_
#define __ServerWorker_H_

#include "./NetSignal.h"

namespace NAME_SPACE {
    
    class ServerWorker {
        
    public:
        /**
         *	@brief	服务器构造函数
         *
         *	@param 	listen_ip 	监听的本地IP
         *	@param 	listen_port 监听的本地端口
         *
         *	@return
         */
        ServerWorker(std::string listen_ip, int listen_port);
        ServerWorker(int listen_port);
        
        ~ServerWorker();
        
        /**
         *	@brief	启动工作
         *
         *	@param
         *
         *	@return
         */
        bool StartWork(TCPServerSignal* pTCPServerSignal);
        
        /**
         *	@brief	停止工作
         *
         *	@param
         *
         *	@return
         */
        void StopWork();
        
        // 获取监听套接字
        SOCKET GetFd() { return _listen_fd; }
        
    public:
        
        /**
         *	@brief	新连接处理，此函数外部禁止调用，用于event事件回调函数调用
         *
         *	@param 	int 套接字句柄
         *	@param 	struct sockaddr_in * 客户端地址
         */
        void Accept(int fd, struct sockaddr_in *sa);
        
        /**
         *	@brief	监听失败事件处理，此函数外部禁止调用，用于event事件回调函数调用
         *
         *	@param 	int 套接字句柄
         *	@param 	EM_NET_EVENT 错误码
         */
        void AcceptError(int fd, EM_NET_EVENT msg);
        
    private:
        // 事件监听器
        evconnlistener* _listener;
        // 监听的IP
        std::string _listen_ip;
        // 监听的端口
        unsigned short _listen_port;
        // 监听的套接字
        SOCKET _listen_fd;
        // 连接器
        TCPServerSignal* _pTCPServerSignal;
    };
    
}

#endif /* defined(__ServerWorker_H_) */
