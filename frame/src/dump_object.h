/// *****************************************************************
/// @file    dump_object.h
///
/// @warning    Copyright (C), 1993-2010, bafei  Information Technologies Co., Ltd.
///
/// @brief        此文件中定义了对象状态监控的接口类@c CDumpObject 。
///
/// @attention  
///
/// @todo     
///
/// @version    1.0
///
/// @author        mone
///
/// @date        2010-04-30
///
/// @note        <author>    <date>            <note>  \n       
///
/// *****************************************************************
#ifndef __DUMP_OBJECT_H_
#define __DUMP_OBJECT_H_

#include <string>
#include <ace/Process_Mutex.h>


#define PRINTF_CONTENT_LEN 2048

/// @class CDumpObject
/// @brief 此类是对象状态监控的抽象基类。所有需要在运行时被进行状态查询的类都需要
///        继承于此类，实现相应的纯虚函数。
///        继承于此类的子类，还需要将自己注册到对象状态监控工厂类
/// 
///        @c CObjectMonitorFactory。由该类来执行查看@c CDumpObject子类的状态。
class CDumpObject
{
protected:
    std::string m_class_name;        //    保存类名的
public:
    /// @brief 默认构造函数
    CDumpObject (void) : m_class_name("CDumpObject"){};

    /// @brief 默认析构函数
    virtual ~CDumpObject () {};

    /// @brief    get_class_name
    /// @datail    返回此类的类名字符串。注：类名必须与实际类名一致。具体的
    ///            子类需要重载此函数。
    /// @return 返回指向类名的指针
    inline const char* get_class_name(void) const
    {
        return this->m_class_name.c_str();
    }

    /// @brief dump
    /// @datail 状态信息打印函数。子类根据自己的实际情况实现此纯虚函数，打印
    ///         自己类内部的所有成员变量值、线程运行情况、类的其他状态等。
    /// @attention 注意：以下两个参数是预留给用户的，在用 -w 输入类名的同时，
    ///         根据需要可以再传入一个或者两个参数。但是也可不传入任何附加参数。
    ///         子类的实现该函数时，请对传入参数进行有效保护检查。
    /// @param[in] first_param 第一个参数
    /// @param[in] second_param 第二个参数
    /// @return 命令是否处理成功。0:成功；其它值:处理失败
    ///         返回值为非0时，会打印“未知的命令参数。”调用一次help_inf()函数，以打印帮助信息
    virtual int dump(const char* first_param = NULL, const char* second_param = NULL) = 0;

    /// @brief 打印帮助信息
    virtual void help_info(){};

    /// @brief 采用日志的方式打印状态信息。
    ///        该函数将会把需要打印的信息写入模块的日志文件中。
    /// @param[in] printf_format 将要打印的格式化字符串
    /// @param[in] ... 与格式化字符串相关的参数
    /// @return false，失败；true，成功
    static bool file_printf(const char *printf_format, ... );

};

#define FILE_PRINTF CDumpObject::file_printf

#endif    //__DUMP_OBJECT_H_

