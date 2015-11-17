#include "./signal_event_handler.h"
#include "../console_resource.h"
#include <ucontext.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <glog/logging.h>

namespace NAME_SPACE
{
    namespace frame
    {
        using std::string;

        //////////////////////////////////////////////////////////////////////////
        // linux 信号列表
        const char *(netbsd_signal_names[]) = {
                /* 0 */ 0,
                /* 1 */ "SIGHUP",
                /* 2 */ "SIGINT",
                /* 3 */ "SIGQUIT",
                /* 4 */ "SIGILL",
                /* 5 */ "SIGTRAP",
                /* 6 */ "SIGABRT",
                /* 7 */ "SIGEMT",
                /* 8 */ "SIGFPE",
                /* 9 */ "SIGKILL",
                /* 10 */ "SIGBUS",
                /* 11 */ "SIGSEGV",
                /* 12 */ "SIGSYS",
                /* 13 */ "SIGPIPE",
                /* 14 */ "SIGALRM",
                /* 15 */ "SIGTERM",
                /* 16 */ "SIGURG",
                /* 17 */ "SIGSTOP",
                /* 18 */ "SIGTSTP",
                /* 19 */ "SIGCONT",
                /* 20 */ "SIGCHLD",
                /* 21 */ "SIGTTIN",
                /* 22 */ "SIGTTOU",
                /* 23 */ "SIGIO",
                /* 24 */ "SIGXCPU",
                /* 25 */ "SIGXFSZ",
                /* 26 */ "SIGVTALRM",
                /* 27 */ "SIGPROF",
                /* 28 */ "SIGWINCH",
                /* 29 */ "SIGINFO",
                /* 30 */ "SIGUSR1",
                /* 31 */ "SIGUSR2",
                };

        //////////////////////////////////////////////////////////////////////////

        CSignalEventHandler::CSignalEventHandler(void) : m_sig_handler(NULL)
        {
        }


        CSignalEventHandler::~CSignalEventHandler(void)
        {
            m_sig_handler = NULL;
        }


        void CSignalEventHandler::set_sig_handler( ACE_Sig_Handler *sig_handler )
        {
            this->m_sig_handler = sig_handler;
        }

        void CSignalEventHandler::set_program_name( string program_name )
        {
            this->m_program_name = program_name;
        }

        int CSignalEventHandler::handle_signal ( int signum, siginfo_t *p_siginfo, ucontext_t *p_sigcontext )
        {

            switch( signum )
            {
            case SIGINT:// 处理 INT 信号
            case SIGPIPE:// 处理 PIPE 信号
            {
                break;
            }
            // 处理ABRT信号
            case SIGABRT:
            case SIGSEGV:
            case SIGBUS:
            case SIGEMT:
            case SIGQUIT:
            case SIGILL:
            case SIGTRAP:
            case SIGFPE:
            case SIGSYS:
            case SIGXCPU:
            case SIGXFSZ:
            {
                LOG(ERROR)<<"捕捉到信号:"<<signum;
                this->m_sig_handler->remove_handler(signum);
                raise(signum);
            }
                break;
            default:
            {
                return 0;
            }
            }

            return 0;
        }
    }
}
