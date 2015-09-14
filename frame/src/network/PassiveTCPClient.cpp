//
//  PassiveTCPClient.cpp
//  被动TCP客户端
//
//  Created by chenjianjun on 15/9/7.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//

#include "PassiveTCPClient.h"
#include "NetFrame.h"

namespace NAME_SPACE {
    
    void PassiveTCPTimeOutEventCb(evutil_socket_t fd, short, void *data) {
        
        PassiveTCPClient *pPassiveTCPClient = (PassiveTCPClient*)data;
        // 心跳超时回调
        pPassiveTCPClient->ProcEvent(ENE_HEART_TIMEOUT);
    }
    
    void PassiveTCPReadEventCb(struct bufferevent *bev, void *data) {
        PassiveTCPClient* pPassiveTCPClient = (PassiveTCPClient*)data;
        
//        static char databuf[40960];
//        size_t datalen = 0;
//        size_t nbytes;
        
//        while ((nbytes = evbuffer_get_length(bev->input)) > 0) {
//            evbuffer_remove(bev->input, databuf+datalen, sizeof(databuf)-datalen);
//            datalen += nbytes;
//        }
        
//        // 数据接收回调
//        pPassiveTCPClient->PutRecvData(databuf, datalen);
        static char databuf[RECV_DATA_MAX_PACKET_SIZE];
        PacketLength datalen = 0;
        PacketLength nbytes = 0;

        /** TCP网络通信的时候采用头两个字节为数据包长度的方式进行规范，防止粘包 */
        do {

            nbytes = evbuffer_get_length(bev->input);
            if (nbytes < kPacketLenSize) {
                return;
            }

            // 如果大于系统定义的最大包长度，为防止恶意行为需要做断开处理
            datalen = GetBE32(bev->input);
            if (datalen > RECV_DATA_MAX_PACKET_SIZE) {

                LOG(INFO)<<"接收服务器的数据超过缓冲区大小,断开客户端.收到的数据长度:"<<datalen;
                pPassiveTCPClient->ProcEvent(ENE_CLOSE);

                return;
            }

            // 判断数据是否收集齐全，没有收集齐全的不做处理
            if (nbytes < datalen) {
                return;
            }

            // 取出完整的数据包
            evbuffer_remove(bev->input, databuf, datalen);
            // 数据接收回调
            pPassiveTCPClient->PutRecvData(databuf+kPacketLenSize, datalen);

        } while (true);

    }
    
    void PassiveTCPEventCb(struct bufferevent *bev, short events, void *data) {
        PassiveTCPClient* pPassiveTCPClient = (PassiveTCPClient*)data;
        // 处理事件
        pPassiveTCPClient->ProcEvent(ENE_CLOSE);
    }
    
    PassiveTCPClient::PassiveTCPClient(SOCKET fd, struct sockaddr_in* sa, short heart_time)
    :_fd(fd),
    _client_ip(inet_ntoa(sa->sin_addr)),
    _client_port(ntohs(sa->sin_port)),
    _bev(nullptr),
    _heart_num(0),
    _heart_time(heart_time),
    _pTCPClientSignal(nullptr)
    {}
    
    PassiveTCPClient::~PassiveTCPClient() {
        StopWork();
        _pTCPClientSignal = nullptr;
    }
    
    bool PassiveTCPClient::StartWork(TCPClientSignal* pTCPClientSignal) {
        
        if (_bev) {
            return false;
        }
        
        _bev = bufferevent_socket_new(NetFrame::_base,
                                      _fd,
                                      BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
        if (_bev == nullptr) {
            return false;
        }
        
        _event = event_new(NetFrame::_base,
                           _fd,
                           EV_TIMEOUT|EV_PERSIST,
                           PassiveTCPTimeOutEventCb, this);
        if (_event == nullptr) {
            bufferevent_free(_bev);
            _bev = nullptr;
            return false;
        }

        // 心跳计数
        _heart_num = 0;
        
        _pTCPClientSignal = pTCPClientSignal;
        // 设置心跳检测时间
        struct timeval timeout = {_heart_time, 0};
        event_add(_event, &timeout);
        
        bufferevent_setcb(_bev, PassiveTCPReadEventCb, nullptr, PassiveTCPEventCb, this);
        bufferevent_enable(_bev, EV_READ);
        
        return true;
    }
    
    void PassiveTCPClient::StopWork() {
        
        if (_bev) {
            bufferevent_disable(_bev, EV_READ);
            bufferevent_free(_bev);
            _bev = nullptr;
        }
        
        if (_event) {
            event_del(_event);
            event_free(_event);
            _event = nullptr;
        }
        
        // 不要对_pPassiveTCPClientSignal置null，释放由外部传入者负责
    }
    
    int PassiveTCPClient::SendData(void* pdata, size_t len) {
        
        if (_bev == nullptr) {
            return FUNC_FAILED;
        }

        if (bufferevent_write(_bev, pdata, len) < 0) {
            return FUNC_FAILED;
        }
        
        return FUNC_SUCCESS;
    }
    
    void PassiveTCPClient::PutRecvData(void* data, size_t len) {
        
        // 有数据往来,心跳计数清零
        _heart_num = 0;

        if (_pTCPClientSignal) {
            _pTCPClientSignal->SignalRecvData(_fd, data, len);
        }
    }
    
    void PassiveTCPClient::ProcEvent(EM_NET_EVENT event) {
        
        if (!_pTCPClientSignal) {
            return;
        }
        
        if (ENE_HEART == event) {

            // 如果超过三次，说明服务器已经死了，这个时候需要关闭
            if(++_heart_num > 2) {
                event = ENE_HEART_TIMEOUT;
            }

        }

        _pTCPClientSignal->SignalEvent(_fd, event);
        
    }
}
