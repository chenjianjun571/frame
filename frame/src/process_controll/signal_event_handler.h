///***************************************************************************
/// @Copyright (C), 1993-2010, bafei  Information Technologies Co., Ltd.
///
/// @file SignalEventHandler.h
///
/// @brief 自定义进程信号处理类。捕获进程中收到的一些特殊信号，避免引起进程错误，
///           可在此类中对相关信号进行处理。
///
/// @attention
///
/// @todo
///
/// @version v1.0
///
/// @author mone
///
/// @date 2010/08/18
///
/// @note 
///        <author>        <time>            <version >        <desc>    
///
///**************************************************************************
#ifndef _SIGNAL_EVENT_HANDLER_H_
#define _SIGNAL_EVENT_HANDLER_H_


#include "ace/Signal.h"
#include "ace/Sig_Handler.h"
#include "../base/common.h"

#include <cstdio>
#include <string>

#ifdef  SIGEMT
#undef  SIGEMT
#endif

#define SIGEMT      7

/*
#ifdef  SIGTRAP
#undef  SIGTRAP
#endif

#define SIGTRAP     5
*/

#ifdef  SIGBUS
#undef  SIGBUS
#endif

#define SIGBUS      10

namespace NAME_SPACE
{
    namespace frame
    {
        class CSignalEventHandler : public ACE_Event_Handler
        {
        public:
            CSignalEventHandler(void);

            virtual ~CSignalEventHandler(void);

        protected:
            ACE_Sig_Handler *m_sig_handler;

            std::string m_program_name;

            ///
            /// @brief 获取堆栈信息并写入日志
            /// @param 无
            ///
            void get_stack_info(int signum);

        public:

            ///
            /// @brief 注册需要捕获的信号
            /// @param[in] sig_handler 待捕获的信号对象
            ///
            void set_sig_handler( ACE_Sig_Handler *sig_handler );

            ///
            /// @brief 注册需要捕获的对象模块名称
            /// @param[in] program_name 待捕获的对象模块名称
            ///
            void set_program_name( std::string program_name );

            ///
            /// @brief 处理捕获到的信号
            /// @param[in] signum    信号类型（信号枚举值）
            /// @param[in] siginfo
            /// @param[in] sigcontext
            ///
            /// @return
            virtual int handle_signal ( int signum, siginfo_t *p_siginfo = 0, ucontext_t *p_sigcontext = 0 );
        };
    }
}

#endif
