#ifndef _CAT_SERVICE_H_
#define _CAT_SERVICE_H_

#include <mutex>
#include <thread>
#include <unordered_map>
#include <functional>
#include <tinyMuduo/TcpServer.h>
#include "UserModel.h"
#include "LocationModel.h"
#include "FoodModel.h"
#include "CatModel.h"
#include "CategoryModel.h"
#include "FeatureModel.h"
#include "ActivityModel.h"
#include "CatLocationModel.h"
#include "json.hpp"

using nlohmann::json;
using std::unordered_map;

using MsgHandler = std::function<void(const TcpConnectionPtr &, json &, Timestamp)>;

enum MsgType
{
    LOGIN_MSG = 1, //  登录
    LOGIN_MSG_ACK,
    REG_MSG, //  注册
    REG_MSG_ACK,
    LOGINOUT_MSG, //  登出
    LOGINOUT_MSG_ACK,
    ENROLL_MSG,         //  用户报名活动 7
    ENROLL_MSG_ACK,
    SHOW_MSG,           //  显示有用表信息 9
    SHOW_MSG_ACK,
    CANCEL_MSG,         //  用户取消活动 11
    CANCEL_MSG_ACK,
    UPDATE_MSG,         //  用户登记猫位置 13
    UPDATE_MSG_ACK
};


enum ErrorType
{
    NONE_ERROR = 0,     //  用户操作成功
    REG_ERROR,          //  用户注册失败
    LOGIN_ERROR,        //  用户登录失败
    ENROLL_ERROR,       //  用户报名活动失败
    CANCEL_ERROR,       //  用户取消活动失败
    UPDATE_ERROR,       //  更新猫位置失败
};

//  业务模块
class CatService
{
public:
    //  提供给CatService
    //  单例模式
    static CatService *getInstance();
    //  根据消息类型获取回调函数
    MsgHandler getMsgHandler(const MsgType &msg_type) const;
    //  服务器终止 重置user表中的所有在线状态
    void reset();

    //  业务
    //  注册
    void reg(const TcpConnectionPtr &, json &, Timestamp);
    //  登录
    void login(const TcpConnectionPtr &, json &, Timestamp);
    //  报名活动
    void enroll(const TcpConnectionPtr &, json &, Timestamp);
    //  发送可用信息给user
        //  food table
        //  cat table
        //  cat_location table 
    void show(const TcpConnectionPtr &, json &, Timestamp);
    void cancel(const TcpConnectionPtr &, json &, Timestamp);
    //  登记cat出现位置及信息
    void updateCat(const TcpConnectionPtr &, json &, Timestamp);
private:
    CatService();

private:
    //  <id , handler>
    unordered_map<MsgType, MsgHandler> msgHandlerTable_;
    //  user连接信息
    unordered_map<int, TcpConnectionPtr> userConnTable_;
    //  记录user连接信息
    std::mutex connMtx_;
    //  table 操作类
    UserModel userModel_;
    CatModel catModel_;
    ActivityModel activityModel_;
    LocationModel locationModel_;
    FoodModel foodModel_;
    CategoryModel categoryModel_;
    FeatureModel  featureModel_;
    CatLocationModel catLocationModel_;
};

#endif