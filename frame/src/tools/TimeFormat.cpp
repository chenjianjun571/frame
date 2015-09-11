#include "./TimeFormat.h"
#include "ace/Date_Time.h"

using namespace std;

CTimeEx* CTimeEx::get_instance ()
{
    static CTimeEx _instance;
    return &_instance;
}

void CTimeEx::time2Str(struct tm *lt, char *str, int maxLength, int nFormat)
{
    switch(nFormat)
    {
    case 0: 
        {
            ACE_OS::snprintf(str, maxLength, "%d%02d%02d%02d%02d%02d",
                lt->tm_year + 1900, lt->tm_mon + 1,
                lt->tm_mday, lt->tm_hour,
                lt->tm_min, lt->tm_sec);
        }
        break;
    case 1:
        {
            ACE_OS::snprintf(str, maxLength, "%d-%02d-%02d %02d:%02d:%02d",
                lt->tm_year + 1900, lt->tm_mon + 1,
                lt->tm_mday, lt->tm_hour,
                lt->tm_min, lt->tm_sec);
        }
        break;
    case 2:
        {
            ACE_OS::snprintf(str, maxLength, "%d%-02d%-02d 00:00:00",
                lt->tm_year + 1900, lt->tm_mon + 1,
                lt->tm_mday);
        }
        break;
    case 5:
        {
            ACE_OS::snprintf(str, maxLength, "%d%02d%02d%02d0000",
                lt->tm_year + 1900, lt->tm_mon + 1,
                lt->tm_mday, lt->tm_hour);
        }
        break;
    case 6:
        {
            ACE_OS::snprintf(str, maxLength, "%d%02d%02d%02d%02d%02d",
                lt->tm_year + 1900, lt->tm_mon + 1,
                lt->tm_mday, lt->tm_hour,
                lt->tm_min, lt->tm_sec);
        }
        break;
    case 7:
        {
            ACE_OS::snprintf(str, maxLength, "%02d%02d", 
                lt->tm_mon + 1, lt->tm_mday);
        }
        break;

    case 8:
        {
            ACE_OS::snprintf(str, maxLength, "%d-%02d-%02d",
                lt->tm_year + 1900, 
                lt->tm_mon + 1,
                lt->tm_mday);
        }
        break;
    case 9: 
        {
            ACE_OS::snprintf(str, maxLength, "%02d%02d%02d%02d%02d%02d",
                (lt->tm_year + 1900) % 100, lt->tm_mon + 1,
                lt->tm_mday, lt->tm_hour,
                lt->tm_min, lt->tm_sec);
        }
        break;
    default:
        break;
    }
}

void CTimeEx::getCurTimeStr(char *pTime, int maxLength, int nFormat)
{
    time_t dttime = time(NULL);
    time2str(dttime, pTime, maxLength, nFormat);
}


int CTimeEx::getPartofCurTime(int nType)
{
    struct tm ptm;
    time_t dttime = time(NULL);
    ACE_OS::localtime_r( &dttime, &ptm);
    return getPartofTime(&ptm, nType);

}

int CTimeEx::getPartofTime(struct tm *ptm, int nType)
{
    if(!ptm)
        return 0;

    switch(nType)
    {
    case TM_YEAR:
        return ptm->tm_year + 1900;
    case TM_MONTH:
        return ptm->tm_mon+1;
    case TM_WEEK:
        return ptm->tm_wday;
    case TM_DAY:
        return ptm->tm_mday;
    case TM_HOUR:
        return ptm->tm_hour;
    case TM_MIN:
        return ptm->tm_min;
    case TM_SEC:
        return ptm->tm_sec;
    default:
        return 0;
    }

}

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
time_t CTimeEx::str2Time(const string &str_time, int nFormat)
{
    struct tm when = {0,0,0,0,0,0,0,0,0};
    const int MAX_TIME_LENGTH = 30;
    char buf[MAX_TIME_LENGTH + 1] = {0};
    char tmp[MAX_TIME_LENGTH + 1] = {0};
    const char* const yearfirst = "20";

    if(str_time.empty())
    {
        return 0;
    }

    strncpy(buf, str_time.c_str(), MAX_TIME_LENGTH);
    char *p = NULL;
    
    char *tmp_ptr = NULL;

    switch(nFormat)
    {
    case 1:
        if((p = ACE_OS::strtok_r(buf, "-", &tmp_ptr)) == NULL
            || (when.tm_year = atoi(p) - 1900 ) > 1100
            || when.tm_year < 0
            || (p = ACE_OS::strtok_r(NULL, "-", &tmp_ptr)) == NULL
            || (when.tm_mon = atoi(p) - 1) < 0
            || when.tm_mon > 11
            || (p = ACE_OS::strtok_r(NULL, " ", &tmp_ptr)) == NULL
            || (when.tm_mday = atoi(p)) > 31
            || when.tm_mday < 1
            || (p = ACE_OS::strtok_r(NULL, ":", &tmp_ptr)) == NULL
            || (when.tm_hour = atoi(p)) > 23
            || when.tm_hour < 0
            || (p = ACE_OS::strtok_r(NULL, ":", &tmp_ptr)) == NULL
            || (when.tm_min = atoi(p)) > 59
            || when.tm_min < 0
            || (p = ACE_OS::strtok_r(NULL, "\0", &tmp_ptr)) == NULL
            || (when.tm_sec = atoi(p)) > 59
            || when.tm_sec < 0
            )
            return 0;
        break;
    case 2:
        if((p = ACE_OS::strtok_r(buf, "/", &tmp_ptr)) == NULL
            || (when.tm_year = atoi(p) - 1900 ) > 1100
            || when.tm_year < 0
            || (p = ACE_OS::strtok_r(NULL, "/", &tmp_ptr)) == NULL
            || (when.tm_mon = atoi(p) - 1) < 0
            || when.tm_mon > 11
            || (p = ACE_OS::strtok_r(NULL, " ", &tmp_ptr)) == NULL
            || (when.tm_mday = atoi(p)) > 31
            || when.tm_mday < 1
            || (p = ACE_OS::strtok_r(NULL, ":", &tmp_ptr)) == NULL
            || (when.tm_hour = atoi(p)) > 23
            || when.tm_hour < 0
            || (p = ACE_OS::strtok_r(NULL, ":", &tmp_ptr)) == NULL
            || (when.tm_min = atoi(p)) > 59
            || when.tm_min < 0
            || (p = ACE_OS::strtok_r(NULL, "\0", &tmp_ptr)) == NULL
            || (when.tm_sec = atoi(p)) > 59
            || when.tm_sec < 0
            )
            return 0;
        break;
    case 3:
        if(strncpy(tmp, buf, 4) == NULL
            || (when.tm_year = atoi(tmp) - 1900 ) > 1100
            || when.tm_year < 0
            || strncpy(tmp + 4, buf + 4, 2) == NULL
            || (when.tm_mon = atoi(tmp + 4) - 1) < 0
            || when.tm_mon > 11
            || strncpy(tmp + 6, buf + 6, 2) == NULL
            || (when.tm_mday = atoi(tmp + 6)) > 31
            || when.tm_mday < 1
            || strncpy(tmp + 8, buf + 8, 2) == NULL
            || (when.tm_hour = atoi(tmp + 8)) > 23
            || when.tm_hour < 0
            || strncpy(tmp + 10, buf + 10, 2) == NULL
            || (when.tm_min = atoi(tmp + 10)) > 59
            || when.tm_min < 0
            || strncpy(tmp + 12, buf + 12, 2) == NULL
            || (when.tm_sec = atoi(tmp + 12)) > 59
            || when.tm_sec < 0
            )
            return 0;
        break;
    case 4:
        {
            char firstyear[5] = {0};
            strcpy(firstyear, yearfirst);

            if(strncpy(tmp, buf, 2) == NULL
                || (when.tm_year = atoi(strcat(firstyear, tmp)) - 1900 ) > 1100
                || when.tm_year < 0
                || strncpy(tmp + 2, buf + 2, 2) == NULL
                || (when.tm_mon = atoi(tmp + 2) - 1) < 0
                || when.tm_mon > 11
                || strncpy(tmp + 4, buf + 4, 2) == NULL
                || (when.tm_mday = atoi(tmp + 4)) > 31
                || when.tm_mday < 1
                || strncpy(tmp + 6, buf + 6, 2) == NULL
                || (when.tm_hour = atoi(tmp + 6)) > 23
                || when.tm_hour < 0
                || strncpy(tmp + 8, buf + 8, 2) == NULL
                || (when.tm_min = atoi(tmp + 8)) > 59
                || when.tm_min < 0
                || strncpy(tmp + 10, buf + 10, 2) == NULL
                || (when.tm_sec = atoi(tmp + 10)) > 59
                || when.tm_sec < 0
                )
                return 0;
        }

        break;
    default:
        return 0;
        //break;
    }

    return mktime(&when);
}

//void CTimeEx::get_current_time(otl_datetime* otl_tm,time_t* time_)
//{
//    struct tm pTm;
//
//    ACE_OS::localtime_r(time_, &pTm);
//
//    otl_tm->year = pTm.tm_year + 1900;
//    otl_tm->month = pTm.tm_mon + 1;
//    otl_tm->day   = pTm.tm_mday;
//    otl_tm->hour  = pTm.tm_hour;
//    otl_tm->minute= pTm.tm_min;
//    otl_tm->second= pTm.tm_sec;
//}

/// 
/// @brief    covert_data_time
/// @detail 两种字符串形式的时间格式转换
///            (20100312010101转换为2010-03-12 01:01:01)
/// @param[in]    date_time    yyyymmddhhmiss格式的日期时间字符串
/// @return     yyyy-mm-dd hh:mi:ss格式的日期时间字符串
/// 
string CTimeEx::covert_data_time(const char* date_time)
{
    char str_time[20] = {0};
    str_time[0] = date_time[0];
    str_time[1] = date_time[1];
    str_time[2] = date_time[2];
    str_time[3] = date_time[3];

    str_time[4] = '-';

    str_time[5] = date_time[4];
    str_time[6] = date_time[5];

    str_time[7] = '-';

    str_time[8] = date_time[6];
    str_time[9] = date_time[7];

    str_time[10] = ' ';

    str_time[11] = date_time[8];
    str_time[12] = date_time[9];

    str_time[13] = ':';

    str_time[14] = date_time[10];
    str_time[15] = date_time[11];

    str_time[16] = ':';

    str_time[17] = date_time[12];
    str_time[18] = date_time[13];

    return str_time;
}

char * CTimeEx::StrTok_r(char  * s,const char * tokens,char  ** lasts)
{
    if (s == 0)
        s = *lasts;
    else
        *lasts = s;
    if (*s == 0)                  // We have reached the end
        return 0;
    int l_org = strlen (s);
    
    //s = strtok (s, tokens);
    char *p = NULL;
    s = ACE_OS::strtok_r( s, tokens, &p );

    if (s == 0)
        return 0;
    const int l_sub = strlen (s);
    if (s + l_sub < *lasts + l_org)
        *lasts = s + l_sub + 1;
    else
        *lasts = s + l_sub;
    return s;
}

ACE_INT64 CTimeEx::set_day_lasttime(ACE_INT64 in_time)
{
    ACE_Date_Time data_time(ACE_Time_Value(in_time, 0));
    
    char buffer[30] = {0};

    ACE_OS::snprintf(buffer, sizeof(buffer), "%d%02d%02d235959", 
        data_time.year(),
        data_time.month(),
        data_time.day());
    //效率可以提升
    return str2Time(buffer, 3);
}

int CTimeEx::time2str(ACE_INT64 src_time, char* dest_time, int dest_len, int format_type)
{
    if (NULL == dest_time)
    {
        //入口参数判断
        return -1;
    }

    ACE_Date_Time data_time(ACE_Time_Value(src_time, 0));
    switch(format_type)
    {
    case 0:
        {
            //0: yyyymmddhhmiss
        ACE_OS::snprintf(dest_time, dest_len, "%d%02d%02d%02d%02d%02d", 
            data_time.year(),
            data_time.month(),
            data_time.day(),
            data_time.hour(),
            data_time.minute(),
            data_time.second());
        }
        break;
    case 1:
        {
            //1: yyyy-mm-dd hh:mi:ss
            ACE_OS::snprintf(dest_time, dest_len, "%d-%02d-%02d %02d:%02d:%02d", 
                data_time.year(),
                data_time.month(),
                data_time.day(),
                data_time.hour(),
                data_time.minute(),
                data_time.second());
        }
        break;
    case 2:
        {
            //2: yyyy-mm-dd 00:00:00
            ACE_OS::snprintf(dest_time, dest_len, "%d%-02d%-02d 00:00:00", 
                data_time.year(),
                data_time.month(),
                data_time.day());
        }
        break;
    case 3:
        {
            ////3: yyyy-mm-dd hh:00:00
            ACE_OS::snprintf(dest_time, dest_len, "%d%-02d%-02d %02d:00:00", 
                data_time.year(),
                data_time.month(),
                data_time.day(),
                data_time.hour());
        }
        break;
    case 4:
        {
            //yyyymmdd000000
            ACE_OS::snprintf(dest_time, dest_len, "%d%02d%02d000000", 
                data_time.year(),
                data_time.month(),
                data_time.day());
        }
        break;
    case 5:
        {
            //5: yyyymmddhh0000
            ACE_OS::snprintf(dest_time, dest_len, "%d%02d%02d%02d0000", 
                data_time.year(),
                data_time.month(),
                data_time.day(),
                data_time.hour());
        }
        break;
    case 6:
        {
            //6: yyyymmddhhdd
            ACE_OS::snprintf(dest_time, dest_len, "%d%02d%02d%02d%02d%02d", 
                data_time.year(),
                data_time.month(),
                data_time.day(),
                data_time.hour(),
                data_time.minute(),
                data_time.second());
        }
        break;
    case 7:
        {
            //7: mmdd
            ACE_OS::snprintf(dest_time, dest_len, "%02d%02d", 
                data_time.month(),
                data_time.day());
        }
        break;
    case 8:
        {
            //8:yyyy-mm-dd
            ACE_OS::snprintf(dest_time, dest_len, "%d-%02d-%02d", 
                data_time.year(),
                data_time.month(),
                data_time.day());
        }
        break;
    case 9:
        {
            //9: yymmddhhmiss
            ACE_OS::snprintf(dest_time, dest_len, "%02d%02d%02d%02d%02d%02d", 
                data_time.year() % 100,
                data_time.month(),
                data_time.day(),
                data_time.hour(),
                data_time.minute(),
                data_time.second());
        }
        break;
        //格式: "HH:NN:SS"
        //时间：2010-11-15，修改人：何晋平，关联问题单：786
        //修改原因：调整格式
    case 10:
        {
            //10: yHH:NN:SS
            ACE_OS::snprintf(dest_time, dest_len, "%02d:%02d:%02d", 
                data_time.hour(),
                data_time.minute(),
                data_time.second());
        }
        break;
    default:
        return -1;
    }

    return 0;
}

