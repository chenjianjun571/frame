//
//  Common.h
//  Frame
//
//  Created by chenjianjun on 15/8/31.
//  Copyright (c) 2015年 jsbn. All rights reserved.
//

#ifndef _COMMON_H_
#define _COMMON_H_

#include <cassert>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <memory>
#include <event.h>
#include <event2/listener.h>
#include <glog/logging.h>
#include <mysql++/cpool.h>
#include <mysql++/scopedconnection.h>
#include <mysql++/connection.h>


#define NAME_SPACE jsbn


namespace NAME_SPACE {

#define DISALLOW_ASSIGN(TypeName) \
    void operator=(const TypeName&)

#define DISALLOW_COPY_AND_ASSIGN(TypeName)    \
    TypeName(const TypeName&);                    \
    DISALLOW_ASSIGN(TypeName)

#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) \
    DISALLOW_COPY_AND_ASSIGN(TypeName)

#undef DISALLOW_IMPLICIT_CONSTRUCTORS
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
    TypeName();                                    \
    DISALLOW_EVIL_CONSTRUCTORS(TypeName)

#define LIBJINGLE_DEFINE_STATIC_LOCAL(type, name, arguments) \
    static type& name = *new type arguments

inline void Set8(void* memory, size_t offset, unsigned char v) {
        static_cast<unsigned char*>(memory)[offset] = v;
}

inline unsigned char Get8(const void* memory, size_t offset) {
    return static_cast<const unsigned char*>(memory)[offset];
}

inline void SetBE16(void* memory, unsigned short v) {
    Set8(memory, 0, static_cast<unsigned char>(v >> 8));
    Set8(memory, 1, static_cast<unsigned char>(v >> 0));
}

inline void SetBE32(void* memory, unsigned int v) {
    Set8(memory, 0, static_cast<unsigned char>(v >> 24));
    Set8(memory, 1, static_cast<unsigned char>(v >> 16));
    Set8(memory, 2, static_cast<unsigned char>(v >> 8));
    Set8(memory, 3, static_cast<unsigned char>(v >> 0));
}

inline unsigned short GetBE16(const void* memory) {
    return static_cast<unsigned short>((Get8(memory, 0) << 8) |
                                       (Get8(memory, 1) << 0));
}

inline unsigned int GetBE32(const void* memory) {
    return (static_cast<unsigned int>(Get8(memory, 0)) << 24) |
            (static_cast<unsigned int>(Get8(memory, 1)) << 16) |
            (static_cast<unsigned int>(Get8(memory, 2)) << 8) |
            (static_cast<unsigned int>(Get8(memory, 3)) << 0);
}

inline void SetLE16(void* memory, unsigned short v) {
    Set8(memory, 0, static_cast<unsigned char>(v >> 0));
    Set8(memory, 1, static_cast<unsigned char>(v >> 8));
}

inline void SetLE32(void* memory, unsigned int v) {
    Set8(memory, 0, static_cast<unsigned char>(v >> 0));
    Set8(memory, 1, static_cast<unsigned char>(v >> 8));
    Set8(memory, 2, static_cast<unsigned char>(v >> 16));
    Set8(memory, 3, static_cast<unsigned char>(v >> 24));
}

inline unsigned short GetLE16(const void* memory) {
    return static_cast<unsigned short>((Get8(memory, 0) << 0) |
                                       (Get8(memory, 1) << 8));
}

inline unsigned int GetLE32(const void* memory) {
    return (static_cast<unsigned int>(Get8(memory, 0)) << 0) |
            (static_cast<unsigned int>(Get8(memory, 1)) << 8) |
            (static_cast<unsigned int>(Get8(memory, 2)) << 16) |
            (static_cast<unsigned int>(Get8(memory, 3)) << 24);
}

#define SOCKET int
#define FUNC_SUCCESS  0
#define FUNC_FAILED  -1


typedef enum emNetEvent {
    EVE_UNKNOWN = -1,// 未知错误
    ENE_CONNECTED = 0,// 连接建立
    ENE_HEART,// 心跳
    ENE_HEART_TIMEOUT,// 心跳检测超时
    ENE_ACCEPT_ERROR,// 监听失败
    ENE_CLOSE// 连接关闭
}EM_NET_EVENT;

#define RECV_DATA_MAX_PACKET_SIZE 4096
#define SEND_DATA_MAX_PACKET_SIZE 4096
typedef unsigned int PacketLength;
static const size_t kPacketLenSize = sizeof(PacketLength);

/** TCP网络通信的时候采用头两个字节为数据包长度的方式进行规范，防止粘包 */
//// 智能指针
//typedef struct stRecvData
//{
//    /// 接收数据数据
//    unsigned char recv_buf[RECV_DATA_MAX_PACKET_SIZE];
//    /// 接收数据长度
//    unsigned int recv_len;
//    /// 连接句柄
//    int sock_handle;

//    stRecvData()
//    {
//        recv_len = -1;
//        sock_handle = -1;
//        ::memset(recv_buf, 0, sizeof(recv_buf));
//    }

//}TRecvData;
//// 接收数据智能指针
//typedef std::shared_ptr<TRecvData> sRecvDataPage_ptr;

typedef struct stSendData
{
    /// 发送数据缓冲区
    unsigned char send_buf[SEND_DATA_MAX_PACKET_SIZE];
    /// 发送数据长度
    unsigned int send_len;
    /// 连接句柄
    int sock_handle;

    void Copy(const unsigned char* buf, unsigned int len) {
        // 贴上包头
        SetBE32(send_buf, len);
        // 贴上包体
        ::memcpy(send_buf+kPacketLenSize, buf, (len>SEND_DATA_MAX_PACKET_SIZE) ? SEND_DATA_MAX_PACKET_SIZE : len);

        send_len = kPacketLenSize + (len>SEND_DATA_MAX_PACKET_SIZE) ? SEND_DATA_MAX_PACKET_SIZE : len;
    }
}TSendData;
// 发送数据智能指针
typedef std::shared_ptr<TSendData> sSendDataPage_ptr;

}

#endif
