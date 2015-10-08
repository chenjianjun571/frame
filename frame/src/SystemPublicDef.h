///************************************************************
/// @Copyright (C), 2015-2030, bafei  Information Technologies Co., Ltd.
/// @URL            
/// @file           SystemPublicDef.h
/// @brief          此文件定义整个系统中使用的公共结构定义
/// @attention
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2013年01月07日
/// @Description
/// @History
///************************************************************
#if !defined (__SYSTEM_PUBLIC_DEF_H_FILE__)
#define __SYSTEM_PUBLIC_DEF_H_FILE__

#include "./SystemConstDef.h"
#include <string>

typedef unsigned int uint32;
typedef int int32;
typedef unsigned short uint16;  // NOLINT
typedef short int16;  // NOLINT
typedef unsigned char uint8;
typedef char int8;
typedef uint64_t uint64;
typedef int64_t int64;

inline void Set8(void* memory, size_t offset, uint8 v) {
  static_cast<uint8*>(memory)[offset] = v;
}

inline uint8 Get8(const void* memory, size_t offset) {
  return static_cast<const uint8*>(memory)[offset];
}

inline void SetBE16(void* memory, uint16 v) {
  Set8(memory, 0, static_cast<uint8>(v >> 8));
  Set8(memory, 1, static_cast<uint8>(v >> 0));
}

inline void SetBE32(void* memory, uint32 v) {
  Set8(memory, 0, static_cast<uint8>(v >> 24));
  Set8(memory, 1, static_cast<uint8>(v >> 16));
  Set8(memory, 2, static_cast<uint8>(v >> 8));
  Set8(memory, 3, static_cast<uint8>(v >> 0));
}

inline void SetBE64(void* memory, uint64 v) {
  Set8(memory, 0, static_cast<uint8>(v >> 56));
  Set8(memory, 1, static_cast<uint8>(v >> 48));
  Set8(memory, 2, static_cast<uint8>(v >> 40));
  Set8(memory, 3, static_cast<uint8>(v >> 32));
  Set8(memory, 4, static_cast<uint8>(v >> 24));
  Set8(memory, 5, static_cast<uint8>(v >> 16));
  Set8(memory, 6, static_cast<uint8>(v >> 8));
  Set8(memory, 7, static_cast<uint8>(v >> 0));
}

inline uint16 GetBE16(const void* memory) {
  return static_cast<uint16>((Get8(memory, 0) << 8) |
                             (Get8(memory, 1) << 0));
}

inline uint32 GetBE32(const void* memory) {
  return (static_cast<uint32>(Get8(memory, 0)) << 24) |
      (static_cast<uint32>(Get8(memory, 1)) << 16) |
      (static_cast<uint32>(Get8(memory, 2)) << 8) |
      (static_cast<uint32>(Get8(memory, 3)) << 0);
}

inline uint64 GetBE64(const void* memory) {
  return (static_cast<uint64>(Get8(memory, 0)) << 56) |
      (static_cast<uint64>(Get8(memory, 1)) << 48) |
      (static_cast<uint64>(Get8(memory, 2)) << 40) |
      (static_cast<uint64>(Get8(memory, 3)) << 32) |
      (static_cast<uint64>(Get8(memory, 4)) << 24) |
      (static_cast<uint64>(Get8(memory, 5)) << 16) |
      (static_cast<uint64>(Get8(memory, 6)) << 8) |
      (static_cast<uint64>(Get8(memory, 7)) << 0);
}

inline void SetLE16(void* memory, uint16 v) {
  Set8(memory, 0, static_cast<uint8>(v >> 0));
  Set8(memory, 1, static_cast<uint8>(v >> 8));
}

inline void SetLE32(void* memory, uint32 v) {
  Set8(memory, 0, static_cast<uint8>(v >> 0));
  Set8(memory, 1, static_cast<uint8>(v >> 8));
  Set8(memory, 2, static_cast<uint8>(v >> 16));
  Set8(memory, 3, static_cast<uint8>(v >> 24));
}

inline void SetLE64(void* memory, uint64 v) {
  Set8(memory, 0, static_cast<uint8>(v >> 0));
  Set8(memory, 1, static_cast<uint8>(v >> 8));
  Set8(memory, 2, static_cast<uint8>(v >> 16));
  Set8(memory, 3, static_cast<uint8>(v >> 24));
  Set8(memory, 4, static_cast<uint8>(v >> 32));
  Set8(memory, 5, static_cast<uint8>(v >> 40));
  Set8(memory, 6, static_cast<uint8>(v >> 48));
  Set8(memory, 7, static_cast<uint8>(v >> 56));
}

inline uint16 GetLE16(const void* memory) {
  return static_cast<uint16>((Get8(memory, 0) << 0) |
                             (Get8(memory, 1) << 8));
}

inline uint32 GetLE32(const void* memory) {
  return (static_cast<uint32>(Get8(memory, 0)) << 0) |
      (static_cast<uint32>(Get8(memory, 1)) << 8) |
      (static_cast<uint32>(Get8(memory, 2)) << 16) |
      (static_cast<uint32>(Get8(memory, 3)) << 24);
}

inline uint64 GetLE64(const void* memory) {
  return (static_cast<uint64>(Get8(memory, 0)) << 0) |
      (static_cast<uint64>(Get8(memory, 1)) << 8) |
      (static_cast<uint64>(Get8(memory, 2)) << 16) |
      (static_cast<uint64>(Get8(memory, 3)) << 24) |
      (static_cast<uint64>(Get8(memory, 4)) << 32) |
      (static_cast<uint64>(Get8(memory, 5)) << 40) |
      (static_cast<uint64>(Get8(memory, 6)) << 48) |
      (static_cast<uint64>(Get8(memory, 7)) << 56);
}

//系统运行状态
enum ESysStatus
{
    enStarted = 0,
    enStoped  = 1,
};

// 启动（停止）控制
struct SSystemControl
{
    /// 进程运行状态
    ESysStatus sys_status;

    SSystemControl()
    {
        sys_status = enStarted;
    }
};

// 函数返回值
#ifndef FUNC_SUCCESS
#define FUNC_SUCCESS 0
#endif

#ifndef FUNC_FAILED
#define FUNC_FAILED -1
#endif

#define SOCKET int

typedef enum emNetEvent {
    EVE_UNKNOWN = -1,// 未知错误
    ENE_CONNECTED = 0,// 连接建立
    ENE_HEART,// 心跳
    ENE_HEART_TIMEOUT,// 心跳检测超时
    ENE_CLOSE// 连接关闭
}EM_NET_EVENT;

#define RECV_DATA_MAX_PACKET_SIZE 4096
#define SEND_DATA_MAX_PACKET_SIZE 4096
typedef uint32 PacketLength;
static const size_t kPacketLenSize = sizeof(PacketLength);

typedef struct stSendData
{
    /// 发送数据缓冲区
    unsigned char send_buf[SEND_DATA_MAX_PACKET_SIZE];
    /// 发送数据长度
    unsigned int send_len;
    /// 连接序号
    unsigned short sock_handle;

    void Copy(const unsigned char* buf, unsigned int len) {
        // 贴上包头
        SetBE32(send_buf, len);
        // 贴上包体
        ::memcpy(send_buf+kPacketLenSize, buf, (len>SEND_DATA_MAX_PACKET_SIZE) ? SEND_DATA_MAX_PACKET_SIZE : len);

        send_len = kPacketLenSize + ((len>SEND_DATA_MAX_PACKET_SIZE) ? SEND_DATA_MAX_PACKET_SIZE : len);
    }
}TSendData;
// 发送数据智能指针
typedef std::shared_ptr<TSendData> sSendDataPage_ptr;

#endif //__SYSTEM_PUBLIC_DEF_H_FILE__

