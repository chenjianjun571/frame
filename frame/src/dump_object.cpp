#include <glog/logging.h>
#include "./dump_object.h"

bool CDumpObject::file_printf(const char *printf_format, ... )
{
//    if (NULL == printf_format)
//    {
//        return false;
//    }

//    // 对格式字符串进行组装处理
//    SLogInfo log_info;

//    memset(log_info.content, 0, sizeof(log_info.content));

//    va_list args;

//    va_start( args, printf_format );

//    ACE_OS::vsnprintf( log_info.content, PRINTF_CONTENT_LEN - 1, printf_format, args );

//    va_end( args );

//    log_info.datetime = ACE_OS::time(NULL);
//    //log_info.level = LOG_LEVEL_INFO;
//    log_info.type = LOG_TYPE_USER;
    
//    LOGGER_FACTORY->write_log(&log_info);

//    return true;
}

