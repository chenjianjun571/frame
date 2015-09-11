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
#include <glog/logging.h>
#include <string>
#include <event.h>
#include <event2/listener.h>
#include <mysql++/cpool.h>
#include <mysql++/scopedconnection.h>
#include <mysql++/connection.h>

#define NAME_SPACE jsbn

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

#define SOCKET int
#define FUNC_SUCCESS  0
#define FUNC_FAILED  -1

typedef struct data {
    SOCKET fd;
    char buf[1024];
    size_t len;
}Tdata;

typedef enum emNetEvent {
    EVE_UNKNOWN = -1,// 未知错误
    ENE_CONNECTED = 0,// 连接建立
    ENE_HEART,// 心跳
    ENE_HEART_TIMEOUT,// 心跳检测超时
    ENE_ACCEPT_ERROR,// 监听失败
    ENE_CLOSE// 连接关闭
}EM_NET_EVENT;


#endif
