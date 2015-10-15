/// 
/// @file	IniFile.cpp
///
/// @warning  Copyright (C), 1993-2011, bafei  Information Technologies Co., Ltd.
///
/// @brief	ini文件读取封装类
///
/// @attention	
/// 
///
/// @todo		
///
/// @version	v1.0
///
/// @author 	陈建君
/// @date		2011-07-13
///
/// @note	    <author>	<date>			<note>   \n     
///
#include <fcntl.h>

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include "./ini_file.h"

using namespace std;

// 临时文件名
static const string TMP_FILE = "antispam_ini_tmp";

CIniFile::CIniFile()
{
    m_p_ini_field_head = NULL;
    m_p_ini_field_tail = NULL;
    m_fp = NULL;
    m_filename = "";
}

CIniFile::~CIniFile()
{
    release();

    if (NULL != m_fp)
    {
        fclose(m_fp);
        m_fp = NULL;
    }
}

void CIniFile::release()
{
    CIniField* pIniField = m_p_ini_field_head;
    CIniField* pIniFieldTemp = NULL;

    if(m_p_ini_field_head)
    {
        pIniFieldTemp = m_p_ini_field_head->m_p_next_ini_field;
    }

    while(pIniField)
    {
        delete pIniField;
        pIniField = NULL;

        pIniField = pIniFieldTemp;
        if(pIniField)
        {
            pIniFieldTemp = pIniField->m_p_next_ini_field;
        }
    }

    m_p_ini_field_head = NULL;
    m_p_ini_field_tail = NULL;
}

int CIniFile::read_open(const std::string& str_file_path)
{
    if (str_file_path.empty())
    {
        std::cout << "the file path is empty. file path[" << str_file_path << "]" << std::endl;
        return -1;
    }

    // 按行读文件
    m_fp = fopen(str_file_path.c_str(), "r");
    if(NULL == m_fp)
    {
        std::cout << "open file is error. file path[" << str_file_path << "]" << std::endl;
        return -1;
    }

    release();

    read_file_to_memory();

    fclose(m_fp);
    m_fp = NULL;

    return 0;
}

int CIniFile::create_write_open(const std::string& str_file_path)
{
    if (str_file_path.empty())
    {
        std::cout << "the file path is empty. file path[" << str_file_path << "]" << std::endl;
        return -1;
    }

    m_filename = str_file_path;

    release();

    return 0;
}

int CIniFile::modify_write_open(const std::string& str_file_path)
{
    if (str_file_path.empty())
    {
        std::cout << "the file path is empty. file path[" << str_file_path << "]" << std::endl;
        return -1;
    }

    // 打开写文件
    m_fp = fopen(str_file_path.c_str(), "r");
    if(NULL == m_fp)
    {
        std::cout << "open file is error. file path[" << str_file_path << "]" << std::endl;
        return -1;
    }

    m_filename = str_file_path;

    release();
    read_file_to_memory();

    fclose(m_fp);
    m_fp = NULL;

    return 0;
}

int CIniFile::read_file_to_memory()
{
    string	str_line;
    string str_filed;
    CIniField* p_ini_field = NULL;
    CIniItem* p_ini_item = NULL;
    char buff[512] = {0};

    while(get_line(m_fp, buff, 512, str_line))
    {
        // 取消该行的首尾的空格
        trim(str_line);

        if(str_line.length() <= 0)
        {
            continue;
        }

        if(str_line[0] == ';' || str_line[0] == '#')
        {
            // 是注释
            continue;
        }
        else if(str_line[0] == '[' && str_line[str_line.length() - 1] == ']')
        {
            // 是段
            str_filed = str_line.substr(1, str_line.length() - 2);

            p_ini_field = find_field(str_filed);

            // 查找是否已经添加该段
            if(!p_ini_field)
            {
                // 没有该段就创建段
                p_ini_field = new_field(str_filed);
            }
            else
            {
                // 已经存在该段不处理了
                continue;
            }
        }
        else
        {
            // 是内容
            int n_find = str_line.find('=');
            if(n_find == string::npos || !p_ini_field)
            {
                // 不合格式的类容忽略
                continue;
            }
            else
            {
                string strType = str_line.substr(0, n_find);
                string strValue = str_line.substr(n_find + 1, str_line.length());
                p_ini_field->set_item(trim(strType), trim(strValue));
            }
        }
    }

    return 0;
}

int CIniFile::save()
{
    if (m_fp != NULL || m_filename.empty())
    {
        return -1;
    }

    //unlink(TMP_FILE.c_str());

    // 打开写文件
    m_fp = fopen(TMP_FILE.c_str(), "w+");
    if(NULL == m_fp)
    {
        std::cout << "open temp file is error." << std::endl;
        return -1;
    }

    write_memory_to_file();

    fclose(m_fp);
    m_fp = NULL;

    // 移动文件
    string str_commid = "mv ";
    str_commid += TMP_FILE;
    str_commid += " ";
    str_commid += m_filename;

    if (0 != system(str_commid.c_str()))
    {
        return -1;
    }

    return 0;
}

int CIniFile::write_memory_to_file()
{
    if (NULL == m_fp)
    {
        return -1;
    }

    CIniField* p_ini_field = m_p_ini_field_head;
    CIniItem* p_ini_item = NULL;
    while(p_ini_field)
    {
        fwrite("[", 1, 1, m_fp);
        fwrite(p_ini_field->m_str_name.c_str(), 1, p_ini_field->m_str_name.length(), m_fp);
        fwrite("]\n", 1, 2, m_fp);

        p_ini_item = p_ini_field->m_p_ini_item_head;
        while(p_ini_item)
        {
            fwrite(p_ini_item->m_str_name.c_str(), 1, p_ini_item->m_str_name.length(), m_fp);
            fwrite("=", 1, 1, m_fp);
            fwrite(p_ini_item->m_str_value.c_str(), 1, p_ini_item->m_str_value.length(), m_fp);
            fwrite("\n", 1, 1, m_fp);
            p_ini_item = p_ini_item->m_p_next_ini_item;
        }
        fwrite("\n", 1, 1, m_fp);
        p_ini_field = p_ini_field->m_p_next_ini_field;
    }

    return 0;
}

CIniField* CIniFile::find_field(const string& str_field_name)
{
    CIniField* p_ini_field = m_p_ini_field_head;
    while(p_ini_field)
    {
        if(p_ini_field->m_str_name == str_field_name)
        {
            return p_ini_field;
        }

        if(p_ini_field)
        {
            p_ini_field = p_ini_field->m_p_next_ini_field;
        }
    }
    return p_ini_field;
}

CIniField* CIniFile::new_field(const string& str_field_name)
{
    if(m_p_ini_field_head)
    {
        m_p_ini_field_tail->m_p_next_ini_field = new(nothrow) CIniField(str_field_name);
        m_p_ini_field_tail = m_p_ini_field_tail->m_p_next_ini_field;
    }
    else
    {
        m_p_ini_field_head = new(nothrow) CIniField(str_field_name);
        m_p_ini_field_tail = m_p_ini_field_head;
    }
    return m_p_ini_field_tail;
}

CIniItem* CIniFile::find_item(const string& str_field_name, const string& str_item_name)
{
    CIniItem* p_ini_item = NULL;
    CIniField* p_ini_field = find_field(str_field_name);

    if(p_ini_field)
    {
        p_ini_item = p_ini_field->find_item(str_item_name);
    }

    return p_ini_item;
}

CIniItem* CIniFile::set_item(const string& str_field_name, const string& str_item_name, const string& str_value)
{
    CIniField* p_ini_field = find_field(str_field_name);

    if(p_ini_field)
    {
        return p_ini_field->set_item(str_item_name, str_value);
    }

    return new_item(str_field_name, str_item_name, str_value);
}

CIniItem* CIniFile::new_item(const string& str_field_name, const string& str_item_name, const string& str_value)
{
    CIniField* p_field = new_field(str_field_name);
    if (NULL == p_field)
    {
        return NULL;
    }

    return p_field->set_item(str_item_name, str_value);
}

int CIniFile::get_line(FILE* fp, char* cBuff, int n_buf_len, string& str_line)
{
    char* p_str;
    str_line = "";

    while(1)
    {
        p_str = fgets(cBuff, n_buf_len - 1, fp);
        if(p_str)
        {
            str_line += p_str;
            if(strlen(p_str) != n_buf_len - 1)
            {
                return str_line.length();
            }
        }
        else
        {
            return str_line.length();
        }
    }

    return str_line.length();
}

int CIniFile::get_item_string(const std::string& str_field_name, const std::string& str_item_name, std::string& value)
{
    CIniItem* p_ini_item = find_item(str_field_name, str_item_name);

    if(p_ini_item)
    {
        value = p_ini_item->m_str_value;

        return 0;
    }

    return -1;
}

#if 0
int CIniFile::get_item_int(const string& str_field_name, const string& str_item_name)
{
    std::string value = "";
    get_item_string(str_field_name, str_item_name, value);
    return atoi(value.c_str());
}
#endif

int CIniFile::get_item_int(const std::string& str_field_name, const std::string& str_item_name, int &ret_val)
{
    string str_val;
    if (get_item_string (str_field_name, str_item_name, str_val) < 0)
    {
        return -1;
    }
    int int_tmp = 0;
    bool b_read = false;
    const char *pchar = str_val.c_str ();
    while (NULL != pchar && '\0' != *pchar)
    {
        char ch = *pchar++;
        switch (ch)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                int_tmp *= 10;
                int_tmp += ch - '0';
                break;
            default:
                return -1;
        }
        b_read = true;
    }
    if (!b_read)
    {
        return -1;
    }
    ret_val = int_tmp;
    
    return 0;
}


double CIniFile::get_item_double(const string& str_field_name, const string& str_item_name)
{
    std::string value = "";
    get_item_string(str_field_name, str_item_name, value);
    return atof(value.c_str());
}

string& CIniFile::trim(string &s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    s.erase(s.find_last_not_of("\n") + 1);
    s.erase(s.find_last_not_of("\r") + 1);

    return s;
}
