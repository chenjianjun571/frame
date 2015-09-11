/// 
/// @file	IniFile.h
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
#ifndef _INIFILE_H__
#define _INIFILE_H__

#include <string>
#include <string.h>
#include <stdlib.h>

// ini文件的项
class CIniItem
{
public:
    CIniItem(const std::string& str_item_name, const std::string& str_item_value)
    {
        m_str_name = str_item_name;
        m_str_value = str_item_value;

        m_p_next_ini_item = NULL;
    }

    ~CIniItem(){}

    // 项名
    std::string m_str_name;
    // 项对应的值
    std::string m_str_value;
    // 项链表
    CIniItem*	m_p_next_ini_item;
private:
    CIniItem(){}
    CIniItem(const CIniItem &other);
    CIniItem& operator=(const CIniItem &other);
};

// ini文件的段
class CIniField
{
public:

    CIniField(const std::string& strName)
    {
        m_p_ini_item_head = NULL;
        m_p_ini_item_tail = NULL;
        m_p_next_ini_field = NULL;
        m_str_name = strName;
    }

    ~CIniField()
    {
        release();
    }

    void release()
    {
        CIniItem* p_ini_item = m_p_ini_item_head;
        CIniItem* p_ini_itemTemp;

        if(m_p_ini_item_head)
        {
            p_ini_itemTemp = m_p_ini_item_head->m_p_next_ini_item;
        }

        while(p_ini_item)
        {
            delete p_ini_item;
            p_ini_item = NULL;

            p_ini_item = p_ini_itemTemp;
            if(p_ini_item)
            {
                p_ini_itemTemp = p_ini_item->m_p_next_ini_item;
            }
        }
    }

    CIniItem* find_item(const std::string& str_item_name)
    {
        CIniItem* p_ini_item = m_p_ini_item_head;
        while(p_ini_item)
        {
            if(p_ini_item->m_str_name == str_item_name)
            {
                return p_ini_item;
            }

            if(p_ini_item)
            {
                p_ini_item = p_ini_item->m_p_next_ini_item;
            }
        }
        return p_ini_item;
    }

    CIniItem* new_item(const std::string& str_item_name, const std::string& str_item_value)
    {
        if(m_p_ini_item_tail)
        {
            m_p_ini_item_tail->m_p_next_ini_item = new CIniItem(str_item_name, str_item_value);
            m_p_ini_item_tail = m_p_ini_item_tail->m_p_next_ini_item;
        }
        else
        {
            m_p_ini_item_head = new CIniItem(str_item_name, str_item_value);
            m_p_ini_item_tail = m_p_ini_item_head;
        }

        return m_p_ini_item_tail;
    }

    CIniItem* set_item(const std::string& item, const std::string& value)
    {
        CIniItem* p_ini_item = find_item(item);
        if(p_ini_item)
        {
            p_ini_item->m_str_value = value;
        }
        else
        {
            return new_item(item, value);
        }

        return p_ini_item;
    }

    // 段名
    std::string m_str_name;
    CIniItem* m_p_ini_item_head;
    CIniItem* m_p_ini_item_tail;
    CIniField* m_p_next_ini_field;

private:
    CIniField();
    CIniField(const CIniField &other);
    CIniField& operator=(const CIniField &other);
};

class CIniFile
{
public:
    CIniFile();
    ~CIniFile();

    typedef enum
    {
        READ = 0,
        WRITE = 1,
        READWRITE = 2
    }EOPARENT_MODE;

    // 读取ini文件
    int read_open(const std::string& str_file_path);
    // 写ini文件
    int create_write_open(const std::string& str_file_path);
    // 修改ini文件
    int modify_write_open(const std::string& str_file_path);
    // 保存所写的内容到文件
    int save();
    // 设置项和段值
    CIniItem* set_item(const std::string& str_file_name, const std::string& str_item_name, const std::string& str_value);
    // 根据段和项取相应的值
    int	get_item_string(const std::string& str_field_name, const std::string& str_item_name, std::string& value);
    // 根据段和项取相应的值
    int get_item_int(const std::string& str_field_name, const std::string& str_item_name);
    /// @return 0 操作成功，-1操作失败
    int get_item_int(const std::string& str_field_name, const std::string& str_item_name, int &ret_val);
    // 根据段和项取相应的值
    double get_item_double(const std::string& str_field_name, const std::string& str_item_name);

private:
    //
    int read_file_to_memory();
    //
    int write_memory_to_file();
    // 取文件一行
    int get_line(FILE* fp, char* buff, int n_buf_len, std::string& str_line);
    // 查找相应的段是否存在
    CIniField* find_field(const std::string& str_field_name);
    // 查找相应的项是否存在
    CIniItem* find_item(const std::string& str_field_name, const std::string& str_item_name);
    // 创建一个新段
    CIniField* new_field(const std::string& str_field_name);
    // 创建一个新项
    CIniItem* new_item(const std::string& str_field_name, const std::string& str_item_name, const std::string& str_value);
    // 去掉空格
    std::string& trim(std::string &s);
    // 资源释放
    void release();

private:
    // 文件句柄
    FILE * m_fp;
    // 段链表头
    CIniField* m_p_ini_field_head;
    // 段链表尾
    CIniField* m_p_ini_field_tail;
    // 文件名
    std::string m_filename;
};

#endif
