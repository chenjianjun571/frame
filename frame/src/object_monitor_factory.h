/// *****************************************************************
/// @file    object_monitor_factory.h
///
/// @warning    Copyright (C), 1993-2010, bafei  Information Technologies Co., Ltd.
///
/// @brief        此文件中定义了模块内所有继承了状态查看接口的对象维护以及状态打印类。
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
/// *****************************************************************
#ifndef __OBJECT_STATE_WATCH_FACTORY_H_
#define __OBJECT_STATE_WATCH_FACTORY_H_

#include <list>
#include <map>

#include "./DumpObject.h"

#include "ace/Task.h"
#include "ace/OS.h"
#include "ace/Singleton.h"
#include "ace/streams.h"


///    类名相同的@c CDumpObject子类对象指针链表
typedef std::list<CDumpObject*> TDumpObjectList;

///    @c CDumpObject子类对象的类名与类对象指针映射容器
typedef std::map<std::string, TDumpObjectList> TDumpObjectMap;


/// @class CObjectMonitorFactory
/// @brief 此类是本应用程序的自维护工厂类。在此类中实现对整个应用程序的
///        自维护，比如：各个对象状态查看、各个线程状态监控与维护、内部
///        错误告警。
///        它也是一个主动对象类，继承于@c ACE_Task<ACE_MT_SYNCH>，用于启动相关的线程。
/// 
/// @todo  现只初步实现了对应用程序中某些对象的状态查看、线程状态监控和
///        维护，以后可以继续实现其他自维护功能。
class ACE_Proper_Export_Flag CObjectMonitorFactory : public ACE_Task<ACE_MT_SYNCH>
{
public:
    static CObjectMonitorFactory* instance()
    {
        static CObjectMonitorFactory m_instance;

        return &m_instance;
    }
protected:
    ///    默认构造函数
    CObjectMonitorFactory(void);

    /// 默认析构函数
    ~CObjectMonitorFactory(void);
public:
    /// @brief 主动对象类的启动函数。
    virtual int open(void *args /* = 0 */);

    ///    @brief 主动对象类的各个线程入口
    virtual int svc(void);

    /// @brief 关闭此主动对象
    virtual int fini(void);
public:

    ///
    /// @brief    添加需要被监控的@c CDumpObject 类对象。
    /// @details 在添加的时候，采用类名作为键值，建立类名与类对象的映射，
    ///             如果类名相同的类对象，把它加到同一个映射后的链表上。
    ///    @param[in]    dump_object    新的@c CDumpObject类对象指针。
    /// @return        false ,失败；true , 成功
    ///
    bool attach_dump_object(CDumpObject *dump_object);

    ///
    /// @brief    把某个被监控的@c CDumpObject 类对象从被监控列表中删除。
    ///    @param[in]    dump_object    需要删除的@c CDumpObject类对象指针。
    /// @return        false ,失败；true , 成功
    ///
    bool dettach_dump_object(CDumpObject *dump_object);
private:

    ///
    /// @brief    实时查看@c CDumpObject 类对象状态的线程函数。
    /// @details 定时循环查询共享内存中是否有外部发起的查看某个@c CDumpObject的类
    ///             对象的命令。如果有，则找到此类名对应的类对象，打印出该对象的内部
    ///             状态；如果没有，则继续循环判断。
    /// @return        void
    ///
    void watch_dump_object_thread();

    /// @brief 执行打印同一类名的所有成员对象的状态
    /// @param[in]    class_object_list 同一类名的对象列表
    ///
    void dump_object_of_same_classname(const char* class_name,
        const TDumpObjectList& class_object_list,
        const char* first_param,
        const char* second_param);

    ///
    /// @brief 打印当前可查看的所有类名
    ///
    void print_all_class_names();
private:

    ///    @c CDumpObject对象容器
    TDumpObjectMap m_dump_object_map;

    /// 监控对象容器的互斥锁
    ACE_Thread_Mutex m_objects_mutex;

    /// 控制线程是否运行的布尔值变量
    bool m_is_running;

    FILE * m_std_out;
};

/// 对象状态工厂的单体指针对象宏定义
//#define OBJ_MONITOR ACE_Singleton<CObjectMonitorFactory, ACE_Recursive_Thread_Mutex>::instance()
#define OBJ_MONITOR CObjectMonitorFactory::instance()

#endif//__OBJECT_STATE_WATCH_FACTORY_H_

