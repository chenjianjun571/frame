/// *****************************************************************
/// @file    time_format.h
/// @warning  Copyright (C), 1993-2010, bafei  Information Technologies Co., Ltd.
///
/// @brief        日期时间操作类的定义文件。包括时间的获取，格式化转换等
///
/// @attention
/// @todo    
/// @version    1.0
/// @author     chenjianjun
/// @date        2006-03-09 
/// @note        <author>    <date>            <note>   \n      
///                mone        2010-04-23        整理     \n
/// *****************************************************************
#ifndef __TIME_FORMAT_H
#define __TIME_FORMAT_H

#include <ctime>
#include <string>
#include "ace/OS.h"

// 时间字符串的最大长度
const int gcv_maxlen_timestr = 22;

/// @class CTimeEx
/// @brief 日期时间操作类。包含一些日期时间格式之间、日期时间与字符串之间的
///        转换函数。可根据需要另外添加一些时间操作函数。
class CTimeEx
{
private:
    CTimeEx () { }
public:
    /// 
    /// @brief 获取此类的唯一实例
    static CTimeEx* get_instance ();

    enum TM_TIME
    {
        TM_YEAR,
        TM_MONTH,
        TM_WEEK,
        TM_DAY,
        TM_HOUR,
        TM_MIN,
        TM_SEC
    };

    static void getCurTimeStr(char *pTime, int maxLength, int nFormat = 0);
    static int    getPartofTime(struct tm *ptm, int nType);
    static int    getPartofCurTime(int nType);

    static ACE_INT64 set_day_lasttime(ACE_INT64 in_time);
    /// 
    /// @brief    time2Str
    /// @detail    把tm类型的日期时间值转换为相应的字符串格式。
    /// @param[in]    lt    tm类型的日期时间值
    ///    @param[in]    maxLength    返回的时间字符串最大长度
    ///    @param[in]    nFormat    转换后的字符串形式的日期时间类型：
    ///                    0: yyyymmddhhmiss
    ///                    1: yyyy-mm-dd hh:mi:ss
    ///                    2: yyyy-mm-dd hh:00:00
    ///                    3: yyyy-mm-dd%20hh:00:00 just for php link
    ///                    4: hh:mi
    ///                    5: yyyymmddhh0000
    ///                    6: yyyymmddhhdd
    ///                    7: mmdd
    /// @param[out]    str    返回转换后的日期时间字符串
    /// @return： void
    ///
    static void time2Str(struct tm *lt, char *str, int maxLength, int nFormat);

    /// 
    /// @brief    str2Time
    /// @detail    用于在64位操作系统下，把字符串格式的时间值转换为time_t
    ///            类型的日期时间值。
    /// @param[in]    pTime    字符串形式的日期时间值
    ///    @param[in]    nFormat    转换类型，也即是pTime的格式类型：
    ///                    1,    yyyy-mm-dd hh:mi:ss
    ///                    2,    yyyy/mm/dd hh:mi:ss
    ///                    3,    yyyymmddhhmiss
    ///                    4,    yymmddhhmiss
    /// @return      0，转换失败，可能是时间格式错误；
    ///              > 0, 转换成功，返回time_t的时间类型
    /// 
    //static time_t str2Time(const char *pTime, int nFormat);
    static time_t str2Time(const std::string &str_time, int nFormat);

    //static void get_current_time(otl_datetime* otl_tm,time_t* time_);

    /// 
    /// @brief    covert_data_time
    /// @detail 两种字符串形式的时间格式转换
    ///            (20100312010101转换为2010-03-12 01:01:01)
    /// @param[in]    date_time    yyyymmddhhmiss格式的日期时间字符串
    /// @return     yyyy-mm-dd hh:mi:ss格式的日期时间字符串
    ///
    static std::string covert_data_time(const char* date_time);

    static char * StrTok_r(char  * s,const char * tokens,char  ** lasts);

    ///***********************************************************
    /// @brief 64位的时间值转换为字符串形式的时间
    /// @details 以后逐渐用此函数代替 time2Str
    /// @param[in]: str_time时间值
    /// @param[out]:dest_time保存字符串时间格式的缓存区
    ///         此缓存区的大小至少时19个字节，此时半不会在字符串尾
    ///         添加字符串结束符
    /// @param[in] dest_len dest_time的字符大小
    /// @param[in] format_type 字符串时间格式的长度
    ///            0: yyyymmddhhmiss
    ///            1: yyyy-mm-dd hh:mi:ss
    ///            2: yyyy-mm-dd 00:00:00
    ///            3: yyyy-mm-dd hh:00:00
    ///            (无用)3: yyyy-mm-dd%20hh:00:00 just for php link
    ///            4: yyyymmdd000000
    ///            （无用）4: hh:mi
    ///            5: yyyymmddhh0000
    ///            6: yyyymmddhhdd
    ///            7: mmdd
    ///            8: yyyy-mm-dd
    ///            9: yymmddhhmiss
    ///            10: yHH:NN:SS
    /// @return:0:操作成功；-1:操作失败
    ///***********************************************************/
    static int time2str(ACE_INT64 src_time, char* dest_time, int dest_len, int format_type);

};

#define TIME_TOOLS (CTimeEx::get_instance())

#endif//INFOX_SMMS_TIMEEX_INCLUDE

