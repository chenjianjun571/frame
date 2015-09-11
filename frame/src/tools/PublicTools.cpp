#include "./PublicTools.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include <climits>
#include <string>

using namespace std;

static const int ANTI_MAX_DIR_LENGTH = 256;

CPublicTools* CPublicTools::get_instance ()
{
    static CPublicTools *_instance = NULL;
    if (NULL != _instance)
    {
        _instance = new(nothrow) CPublicTools;
    }
    return _instance;
}

int CPublicTools::trim_blank (const char *istr, char *ostr, int slen, const char *pblank)
{
    if (NULL == istr || NULL == ostr || 0 == slen)
    {
        return 0;
    }

    int nmax = (slen >= 0 ? slen : numeric_limits<int>::max ());
    int i = 0;
    int n = 0;

    while (i++ < nmax && 0 != *istr)
    {
        bool is_blank = false;
        const char ch = *istr++;

        // 判断当前字符串是空白符
        const char *pb = pblank;
        while (0 != *pb)
        {
            if (*(pb++) == ch)
            {
                is_blank = true;
                break;
            }
        }

        // 如果不是空白符，只在到输出缓存区
        if (!is_blank) ostr[n++] = ch;
    }
    return n;
}

//bool CPublicTools::is_dir (const char *dir_name)
//{
//    if (NULL == dir_name)
//    {
//        return false;
//    }
//    ACE_DIR *pdir = ACE_OS::opendir (dir_name);
//    if (NULL == pdir)
//    {
//        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("_DEBUG:line=%l, pid=%P, Info=%m: %s\n"), dir_name));
//        return false;
//    }
//    ACE_OS::closedir (pdir);
//    return true;
//}

bool CPublicTools::is_dir(const char *dir_name)
{
    if (NULL == dir_name)
    {
        return false;
    }
    struct stat file_stat;
    memset(&file_stat, 0, sizeof(struct stat));
    stat(dir_name, &file_stat);
    if (!S_ISDIR(file_stat.st_mode))
    {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("_DEBUG:line=%l, pid=%P, Info=%m: %s\n"), dir_name));
        return false;
    }

    return true;
}

bool CPublicTools::create_dir (const char *dir_name)
{
    if (NULL == dir_name)
    {
        return false;
    }

    // 如果文件夹已经存在，不创建
    if (is_dir (dir_name))
    {
        return true;
    }
    
    // 创建dir
    if (0 != ACE_OS::mkdir (dir_name, ACE_DEFAULT_DIR_PERMS))
    {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT("_DEBUG:Info=%m tid=%t: %s\n"), dir_name));
    }

    // 创建是否成功
    if (is_dir (dir_name))
    {
        return true;
    }
}

bool CPublicTools::mkdir_p(const char *dir_name)
{
    if (NULL == dir_name)
    {
        return false;
    }

    if (strlen(dir_name) >= (ANTI_MAX_DIR_LENGTH - 1))
    {
        printf("超过Anti允许的最大目录长度\n");
        return false;
    }

    // 如果目录已经存在，不创建
    if (is_dir(dir_name))
    {
        return true;
    }

    char dir_path[ANTI_MAX_DIR_LENGTH];
    memset(dir_path, 0, ANTI_MAX_DIR_LENGTH);
    memcpy(dir_path, dir_name, ANTI_MAX_DIR_LENGTH);

    // 如果目录名不是以'/'结束，加上'/'
    int length = strlen(dir_path);
    if (dir_path[length - 1] != '/')
    {
        dir_path[length] = '/';
    }

    char *tmp_ptr = dir_path;
    // 如果是以'/'开头，表示根目录,直接跳过处理后面的目录
    // 如果是'~'开头，会自动转换为/home/用户名 来处理，与以'/'开头相同
    if ('/' == *tmp_ptr)
    {
        ++tmp_ptr;
    }

    // 循环创建各级目录
    while ('\0' != *tmp_ptr)
    {
        if ('/' != *tmp_ptr)
        {
            ++tmp_ptr;
            continue;
        }

        // 如果当前字符是'/' 取出判断前面的字符串是否是目录
        *tmp_ptr = '\0';

        // 如果不是目录
        if (!is_dir(dir_path))
        {
            //  创建目录
            if (0 != ACE_OS::mkdir(dir_path, ACE_DEFAULT_DIR_PERMS))
            {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT("_DEBUG:Info=%m tid=%t: %s\n"), dir_path));
                return false;
            }
        }

        // 继续处理后面的目录
        *tmp_ptr = '/';
        ++tmp_ptr;
    }

    return true;
}

int CPublicTools::find_string_end (const char *pstr, int str_len)
{
    int i = 0;
    while ('\0' != *pstr++ && i < str_len) ++i;
    return i;
}

bool CPublicTools::check_good_account(const char *pstr)
{
    if(match_account(pstr, "98765432109876543210"))
    {
        return true;
    }

    return match_account(pstr, "01234567890123456789");
}

bool CPublicTools::match_account(const char *pstr, const char* p_match_str)
{
    int match_len = strlen(p_match_str);

    const char* pmatch = pstr + 2;// 忽略前面两位前置码

    int seq = pmatch[0]%48;
    if(seq > 9)
    {
        return false;
    }

    char prev = pmatch[0];
    char c;
    bool flg = true;
    bool is86 = true;
    bool isequal = true;
    int j = 0;
    for(int i = seq; i < match_len ; ++i, ++j)
    {
        c = pmatch[j];

        // 判断是否结尾付
        if(!c)
        {
            break;
        }

        // 判断是否数字
        if(!isdigit(c))
        {
            flg  = false;

            break;
        }

        // 86规则和相同数字规则检查
        if(c != '8' && c != '6')
        {
            is86 = false;
        }

        // 判断前后是否一样,判断相同数字
        if(prev != c)
        {
            isequal = false;
        }

        // 判断连续字符
        if(c != p_match_str[i])
        {
            flg  = false;
        }

        prev = c;
    }

    if(flg || is86 || isequal)
    {
        return true;
    }

    return false;
}

bool CPublicTools::check_account(const char *pstr)
{
    if(strlen(pstr) == 0)
    {
        return false;
    }

    const char* pnext = pstr;

    while(*pnext != '\0')
    {
        // 判断是否数字
        if(!isdigit(*pnext))
        {
            return false;
        }

        ++pnext;
    }

    return true;
}
