//
//  db_op_instance.h
//  frame
//
//  Created by chenjianjun on 15/9/10.
//
//

#ifndef __DB_OP_INSTANCE_H_
#define __DB_OP_INSTANCE_H_

#include "../base/common.h"
#include "../base/critical_section.h"
#include <string>

typedef std::shared_ptr<mysqlpp::ScopedConnection> ScopedConnectionPtr;

namespace NAME_SPACE {
    
    class DBPool;
    class DBOpInstance {
        
    public:
        // 单例接口
        static DBOpInstance* Instance();
        
        // 初期话连接信息
        bool EnvInit(std::string db_name,
                     std::string db_host,
                     std::string db_user,
                     std::string db_passwd,
                     unsigned short max_con_num,
                     unsigned short db_port = 3376);
        
        void Exit();
        
        ScopedConnectionPtr GetConnect();
        
    protected:
        DBOpInstance();
        ~DBOpInstance();
        
    private:
        DBPool* _p_db_pool;
        CriticalSection _sc;
    };
    
}

#endif /* defined(__DBOpInstance_H_) */
