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

#include "../SystemPublicDef.h"

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

}

#endif
