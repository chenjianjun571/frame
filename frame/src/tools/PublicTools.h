///************************************************************
/// @Copyright (C), 1993-2012, bafei  Information Technologies Co., Ltd.
/// @URL            
/// @file           src/frame/tools/PublicTools.h
/// @brief          公共工具类，用于提供常用的公共函数
/// @attention      
/// @Author         chenjianjun
/// @Version        0.1
/// @Date           2012年12月27日 星期四 09时25分50秒
/// @Description    
/// @History        
///************************************************************

#if !defined (__PUBLIC_TOOLS_FILE_HEADER__)
#define __PUBLIC_TOOLS_FILE_HEADER__


/// @class CPublicTools
/// @brief 公共工具类
class CPublicTools
{
public:
    static CPublicTools* get_instance ();

    /// @brief 从字符串剔除空白字符
    /// @details 
    /// @param[in] istr 待处理的字符串
    /// @param[out] ostr 存储处理后的字符串的缓存区
    /// @param[in] slen 要处理的字符串长度, 如果此字符串>=0, 表示要处理的字符的长度
    /// @param[in] pblank 定义的空白符
    /// @return 处理后的字符串的长度, 即ostr中存储的字符的长度
    /// @author lijing
    /// @date 2012年12月27日 星期四 09时29分42秒
    static int trim_blank (const char *istr, char *ostr, int slen = -1, const char *pblank = " \t\n\r");

    /// @brief 判断否是文件夹
    /// @param[in] dir_name 文件夹名字
    /// @return true是文件夹，false 不是文件夹
    static bool is_dir (const char *dir_name);

    /// @brief 创建文件夹
    /// @return false 创建失败，true创建成功
    static bool create_dir (const char *dir_name);

    /// @brief 创建文件夹 如果父目录不存在也会自动创建
    ///        支持绝对路径和相对路径
    /// @return false 创建失败， true创建成功
    static bool mkdir_p(const char *dir_name);

    /// @brief 找出字符串结束符到字符串第一个字符的距离
    /// @param[in] pstr要处理的字符串
    /// @param[in] str_len字符串的长度, 此函数只会在pstr的前str_len个字符中找字符串结束符
    /// @return 字符串结束到第一个字符串的长度, 此值小于等于str_len
    /// @author lijing
    /// @date 2013年01月16日 星期三 14时01分56秒
    static int find_string_end (const char *pstr, int str_len);

    /// @brief 检查一个号码是否是靓号
    ///
    /// @return true:是靓号 false:不是靓号
    static bool check_good_account(const char *pstr);

    /// @brief 检查账号合法性
    ///
    /// @return true:合法 false:不合法
    static bool check_account(const char *pstr);

protected:
    static bool match_account(const char *pstr,  const char* p_match_str);
};

#define PUBLIC_TOOLS (CPublicTools::get_instance())

#endif //__PUBLIC_TOOLS_FILE_HEADER__

