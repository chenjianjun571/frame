namespace cpp jsbn.rpc.bc
namespace java com.jsbn.rpc.bc

// 城市ID
enum CityID {
    CID_INIT = -1,
    CID_CQ,
    CID_CD,
    CID_HZ
}

struct UserInfo {
    1: required i32 userId;
    2: required string userName;
    3: required string text;
    4: optional CityID cityID = CityID.CID_INIT;
    16: optional string language = "english";
}

typedef list<UserInfo> UserInfoList

struct UserInfoSearchResult {
    1: UserInfoList userinfos;
}

exception Unavailable {
    1: string message;
}

service RpcService {

    /*
    */
    bool postUserInfo(1:UserInfo userInfo) throws (1:Unavailable unavailable),

    /**/
    UserInfoSearchResult searchUserInfos(1:string query);

    /*
    ”oneway”标识符表示client发出请求后不必等待回复（非阻塞）直接进行下面的操作，
    ”oneway”方法的返回值必须是void
    */
    oneway void hello()

}

