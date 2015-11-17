#include "RpcServiceHandler.h"
#include "module_const_def.h"

RpcServiceHandler::RpcServiceHandler()
{}

bool RpcServiceHandler::postUserInfo(const UserInfo& userInfo)
{
    LOG(INFO)<<"postUserInfo被调用,["
            <<userInfo.userId<<","
              <<userInfo.userName<<","
                <<userInfo.text<<","
                  <<userInfo.cityID<<","
                    <<userInfo.language<<"]";

    Unavailable unavailable;
    unavailable.message = "这是服务器抛出的异常";

    throw unavailable;

    return true;
}

void RpcServiceHandler::searchUserInfos(UserInfoSearchResult& _return, const std::string& query)
{
    UserInfoList userInfos;

    for(int i = 0; i < 30000; ++i)
    {
        UserInfo userInfo;
        userInfo.userId = i;
        userInfo.userName = "我是服务器";
        userInfo.language = "chian";
        userInfo.cityID = CityID::type::CID_CQ;
        userInfo.text = "阿海珐哈佛卡拉回访老客户分类客户发货";
        userInfos.push_back(userInfo);
    }

    _return.__set_userinfos(userInfos);

    LOG(INFO)<<"searchUserInfos被调用"<<query;

    {
        try
        {
//            ScopedConnectionPtr scp = jsbn::DBOpInstance::Instance()->GetConnect();
//            if(nullptr == scp)
//            {
//                LOG(ERROR)<<"获取数据库连接失败";
//                return;
//            }
            ScopedConnectionPtr scp = nullptr;
            mysqlpp::Query query((*scp)->query("select VIDEO_ID,NAME,REMARK,URL,IS_USED from jsbn_video"));
            LOG(INFO)<<"1::::::::::::";
            if (mysqlpp::StoreQueryResult res = query.store())
            {
                for (size_t i = 0; i < res.num_rows(); ++i)
                {
                    LOG(INFO)<<res[i];
                }
            }
            LOG(INFO)<<"2::::::::::::";



//LOG(INFO)<<"1::::::::::::";
//            mysqlpp::Query query = (*scp)->query("select VIDEO_ID,NAME,REMARK,URL,IS_USED from jsbn_video");
//            std::vector<stock> res;
//            query.storein(res);
//            std::vector<stock>::iterator it;
//            for (it = res.begin(); it != res.end(); ++it) {
//                LOG(INFO)<<*it;
//            }
//LOG(INFO)<<"2::::::::::::";
        }
        catch(const mysqlpp::BadQuery& e)
        {
            LOG(ERROR)<<"检索失败:"<<e.what();
        }
        catch (const mysqlpp::Exception& er)
        {
            LOG(ERROR)<<"失败:"<<er.what();
        }
        catch (...)
        {
            LOG(ERROR)<<"未知错误";
        }
        LOG(INFO)<<"3::::::::::::";
    }
}

void RpcServiceHandler::hello()
{
    LOG(INFO)<<"hello被调用";
}
