//
//  ActiveTCPClient.cpp
//  Frame
//
//  Created by chenjianjun on 15/9/8.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//

#include "ActiveTCPClient.h"
#include "NetFrame.h"

namespace NAME_SPACE {
    
    void ActiveTCPTimeOutEventCb(evutil_socket_t fd, short, void *data) {
        ActiveTCPClient* pActiveTCPClient = (ActiveTCPClient*)data;
        pActiveTCPClient->ProcEvent(ENE_HEART);
    }
    
    void ActiveTCPEventCb(struct bufferevent *bev, short events, void *data) {
        ActiveTCPClient* pActiveTCPClient = (ActiveTCPClient*)data;
        if (events & BEV_EVENT_CONNECTED) {
            pActiveTCPClient->ProcEvent(ENE_CONNECTED);
        } else {
            pActiveTCPClient->ProcEvent(ENE_CLOSE);
        }
        // (events & (BEV_EVENT_READING|BEV_EVENT_WRITING|BEV_EVENT_EOF|BEV_EVENT_TIMEOUT))
    }
    
    void ActiveTCPReadEventCb(struct bufferevent *bev, void *data) {
        
        ActiveTCPClient* pActiveTCPClient = (ActiveTCPClient*)data;
        
        static char databuf[40960];
        size_t datalen = 0;
        size_t nbytes;
        
        while ((nbytes = evbuffer_get_length(bev->input)) > 0) {
            evbuffer_remove(bev->input, databuf+datalen, sizeof(databuf)-datalen);
            datalen += nbytes;
        }
        
        // 有数据往来，设置标志
        pActiveTCPClient->SetHeartFlg(true);
        
        // 数据接收回调
        pActiveTCPClient->PutRecvData(databuf, datalen);
        
    }
    
    ActiveTCPClient::ActiveTCPClient(std::string host_name, unsigned short host_port, short heart_time)
    :_host_name(host_name),
    _host_port(host_port),
    _heart_time(heart_time),
    _bev(nullptr),
    _pTCPClientSignal(nullptr),
    _heart_flg(false),
    _heart_num(0),
    _event(nullptr),
    _connect_flg(0),
    _m_rw_loacl(RWLock::Create()) {
    }
    
    ActiveTCPClient::~ActiveTCPClient() {
        StopWork();
        _pTCPClientSignal = nullptr;
        delete _m_rw_loacl;
    }
    
    bool ActiveTCPClient::StartWork() {
        
        WriteLockScoped wLock(*_m_rw_loacl);
        
        if (_bev) {
            return false;
        }
        
        _fd = socket(AF_INET, SOCK_STREAM, 0);
        evutil_make_socket_nonblocking(_fd);
        if (_fd < 0) {
            return false;
        }
        
        _bev = bufferevent_socket_new(NetFrame::_base, _fd, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
        if (_bev == nullptr) {
            close(_fd);
            return false;
        }
        
        struct sockaddr_in sSvrAddr;
        memset(&sSvrAddr, 0, sizeof(sSvrAddr));
        sSvrAddr.sin_family = AF_INET;
        sSvrAddr.sin_addr.s_addr = inet_addr(_host_name.c_str());
        sSvrAddr.sin_port = htons(_host_port);
        int addrlen = sizeof(struct sockaddr_in);
        
        // 置为连接中状态
        _connect_flg = 1;
        if (bufferevent_socket_connect(_bev, (struct sockaddr*)&sSvrAddr, addrlen) < 0) {
            _connect_flg = 0;
            StopWork();
            return false;
        }
        
        bufferevent_setcb(_bev, ActiveTCPReadEventCb, nullptr, ActiveTCPEventCb, this);
        bufferevent_enable(_bev, EV_READ);
        
        return true;
    }
    
    void ActiveTCPClient::StopWork() {
        
        WriteLockScoped wLock(*_m_rw_loacl);
        
        _connect_flg = 0;
        if (_event) {
            event_del(_event);
            event_free(_event);
            _event = nullptr;
        }
        
        if (_bev) {
            bufferevent_disable(_bev, EV_READ);
            bufferevent_free(_bev);
            _bev = nullptr;
            _fd = -1;
        }
    }
    
    int ActiveTCPClient::SendData(void* pdata, size_t len) {
        
        ReadLockScoped rLock(*_m_rw_loacl);
        
        if (_bev == nullptr || _connect_flg != 2) {
            return FUNC_FAILED;
        }

        if (bufferevent_write(_bev, pdata, len) < 0) {
            return FUNC_FAILED;
        }
        
//        if (send(_fd, pdata, len, 0) < 0) {
//            return FUNC_FAILED;
//        }
        
        return FUNC_SUCCESS;
        
    }
    
    void ActiveTCPClient::ProcEvent(EM_NET_EVENT event) {
        
        if (!_pTCPClientSignal) {
            return;
        }

        if (ENE_CONNECTED == event) {
            // 已连接状态
            _connect_flg = 2;
            // 连接建立，开启心跳计数
            _event = event_new(NetFrame::_base,
                               _fd,
                               EV_TIMEOUT|EV_PERSIST,
                               ActiveTCPTimeOutEventCb,
                               this);

            // 设置心跳检测时间
            struct timeval timeout = {_heart_time, 0};
            event_add(_event, &timeout);

            // 心跳计数清零
            _heart_num = 0;
        }

        if (ENE_HEART == event) {

            // 如果超过三次，说明服务器已经死了，这个时候需要关闭
            if(++_heart_num > 2) {
                event = ENE_HEART_TIMEOUT;
            }

        }
        
        _pTCPClientSignal->SignalEvent(_fd, event);
    }
    
    void ActiveTCPClient::PutRecvData(void* data, size_t len) {
        
        // 有数据往来,心跳计数清零
        _heart_num = 0;

        if (_pTCPClientSignal) {
            _pTCPClientSignal->SignalRecvData(_fd, data, len);
        }
        
    }
}
