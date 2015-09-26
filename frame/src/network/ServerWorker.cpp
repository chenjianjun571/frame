//
//  ServerWorker.cpp
//  Frame
//
//  Created by chenjianjun on 15/9/7.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//

#include "ServerWorker.h"
#include "NetFrame.h"

namespace NAME_SPACE {
    
    // 客户端连接事件回调处理函数
    static void ListenerEventCb(evconnlistener *listener, evutil_socket_t fd,
                                sockaddr *sa, int socklen, void *user_data) {
        ServerWorker *pServerWorker = (ServerWorker*)user_data;
        struct linger l;
        l.l_onoff = 1;
        l.l_linger = 0;
        setsockopt(fd, SOL_SOCKET, SO_LINGER, (void*)&l, sizeof(l));
        pServerWorker->Accept(fd, (struct sockaddr_in *)sa);
    }
    
    // 监听失败回调处理函数
    static void ListenerErrorCb(struct evconnlistener *listener, void *user_data) {
        ServerWorker* pServerWorker = (ServerWorker*)user_data;
        pServerWorker->AcceptError(pServerWorker->GetFd(),ENE_ACCEPT_ERROR);
                                   //evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
    }
    
    ServerWorker::ServerWorker(std::string listen_ip, int listen_port)
    :_listen_ip(listen_ip),
    _listen_port(listen_port),
    _listener(nullptr),
    _pTCPServerSignal(nullptr) {}
    
    ServerWorker::ServerWorker(int listen_port)
    :_listen_port(listen_port),
    _listener(nullptr),
    _pTCPServerSignal(nullptr) {
        _listen_ip.clear();
    }
    
    bool ServerWorker::StartWork(TCPServerSignal* pTCPServerSignal) {
        
        if (_listener) {
            return false;
        }
        
        sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        if (!_listen_ip.empty()) { sin.sin_addr.s_addr = ::inet_addr(_listen_ip.c_str()); }
        sin.sin_port = htons(_listen_port);
        
        _listener = evconnlistener_new_bind(NetFrame::_base,
                                            ListenerEventCb,
                                            (void*)this,
                                            LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE,
                                            -1,
                                            (sockaddr*)&sin,
                                            sizeof(sockaddr_in));
        if( nullptr == _listener ) {
            LOG(ERROR)<<"创建监听器失败,IP["<<_listen_ip<<":"<<_listen_port<<"]"<<evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR());
            return false;
        }
        
        _pTCPServerSignal = pTCPServerSignal;
        
        // 设置监听失败回调
        evconnlistener_set_error_cb(_listener, ListenerErrorCb);
        
        // 获取监听的套接字句柄
        _listen_fd = evconnlistener_get_fd(_listener);
        
        return true;
    }
    
    void ServerWorker::StopWork()
    {
        if (_listener) {
            evconnlistener_free(_listener);
            _listener = nullptr;
        }
    }
    
    void ServerWorker::Accept(int fd, struct sockaddr_in *sa) {
        if (_pTCPServerSignal) {
            _pTCPServerSignal->SignalAccept(fd, sa);
        }
    }
    
    void ServerWorker::AcceptError(int fd, EM_NET_EVENT msg) {
        LOG(ERROR)<<"监听失败.";
    }
    
}
