//
//  db_pool.h
//  frame
//
//  Created by chenjianjun on 15/9/10.
//
//

#ifndef __DBPool_H_
#define __DBPool_H_

#include "../base/common.h"

namespace NAME_SPACE {
    
    typedef struct stDBParameter {

        // 最大连接数
        unsigned short max_con_num;
        // 数据库名
        std::string db_name;
        // 数据库地址
        std::string server_ip;
        // 数据库监听端口
        unsigned int server_port;
        // 数据库用户名
        std::string user;
        // 数据库密码
        std::string password;
        
        stDBParameter() {
            max_con_num = 20;
        }
        
    }TDBParameter;
    
    
    class DBPool:public mysqlpp::ConnectionPool {
        
    public:
        explicit DBPool(TDBParameter&);
        ~DBPool();
        
        // 重写父类的方法
        virtual void release(const mysqlpp::Connection* pc);
        
        // 重写父类的方法
        virtual mysqlpp::Connection* grab();
        
        // 重写父类的方法
        virtual mysqlpp::Connection* safe_grab();

        // 检测连接数
        bool checkUseNumIsOver() { return _con_in_use < _db_parameter.max_con_num; }
        
    protected:
        
        // 实现父类的虚函数
        virtual mysqlpp::Connection* create();
        
        // 实现父类的虚函数
        virtual void destroy(mysqlpp::Connection*);
        
        // 实现父类的虚函数
        virtual unsigned int max_idle_time();
        
    private:
        DBPool(const DBPool& no_copies);
        const DBPool& operator=(const DBPool& no_copies);
        
    private:
        // 数据库参数
        TDBParameter _db_parameter;

        // 连接使用数
        unsigned short _con_in_use;
    };
    
}

#endif /* defined(__DBPool_H_) */
