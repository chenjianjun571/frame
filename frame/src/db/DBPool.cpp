//
//  DBPool.cpp
//  mysql++
//
//  Created by chenjianjun on 15/9/10.
//
//

#include "DBPool.h"

namespace NAME_SPACE {
    
    DBPool::DBPool(TDBParameter& var):_db_parameter(var),_con_in_use(0) {}
    
    DBPool::~DBPool() {
        clear(true);
    }
    
    void DBPool::release(const mysqlpp::Connection* pc) {
        mysqlpp::ConnectionPool::release(pc);
        --_con_in_use;
    }
    
    mysqlpp::Connection* DBPool::grab() {
        
        if (_con_in_use > _db_parameter.max_con_num) {
            LOG(ERROR)<<"超过数据最大连接数";
            return nullptr;
        }
        
        ++_con_in_use;
        return mysqlpp::ConnectionPool::grab();
    }
    
    mysqlpp::Connection* DBPool::safe_grab() {
        
        if (_con_in_use > _db_parameter.max_con_num) {
            LOG(ERROR)<<"超过数据最大连接数";
            return nullptr;
        }

        ++_con_in_use;
        return mysqlpp::ConnectionPool::safe_grab();;
    }
    
    mysqlpp::Connection* DBPool::create()
    {
        mysqlpp::Connection* pCon = new mysqlpp::Connection();
        
        try {
            pCon->connect(_db_parameter.db_name.c_str(),
                          _db_parameter.server_ip.c_str(),
                          _db_parameter.user.c_str(),
                          _db_parameter.password.c_str(),
                          _db_parameter.server_port);
        } catch (mysqlpp::Exception e) {
        }
        
        return pCon;
    }
    
    void DBPool::destroy(mysqlpp::Connection* cp)
    {
        if (cp) {
            delete cp;
        }
    }
    
    unsigned int DBPool::max_idle_time()
    {
        // 把我们的空闲时间设置为60秒。返回服务器的连接空闲超时时间。
        return 60;
    }


}
