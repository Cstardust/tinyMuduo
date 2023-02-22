#include "CatService.h"
#include "User.hpp"


//  注册handler
CatService::CatService()
{
    msgHandlerTable_.insert({MsgType::REG_MSG,std::bind(&CatService::reg,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    msgHandlerTable_.insert({MsgType::LOGIN_MSG,std::bind(&CatService::login,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    msgHandlerTable_.insert({MsgType::ENROLL_MSG,std::bind(&CatService::enroll,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    msgHandlerTable_.insert({MsgType::SHOW_MSG,std::bind(&CatService::show,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    msgHandlerTable_.insert({MsgType::CANCEL_MSG,std::bind(&CatService::cancel,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
    msgHandlerTable_.insert({MsgType::UPDATE_MSG,std::bind(&CatService::updateCat,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3)});
}


MsgHandler CatService::getMsgHandler(const MsgType& msg_type) const
{
    const unordered_map<MsgType,MsgHandler>::const_iterator citer = msgHandlerTable_.find(msg_type);
    if(citer == msgHandlerTable_.end())
    {
        return [msg_type](const TcpConnectionPtr& ,json &,const Timestamp & )
        {
            LOG_INFO("no handler corresponding %d\n",msg_type);
        };
    }
    else
    {
        return citer->second;
    }
}

CatService* CatService::getInstance()
{
    static CatService cat_service;
    return &cat_service;
}


void CatService::login(const TcpConnectionPtr& conn,json & js,Timestamp time)
{
    LOG_INFO("do login service ! ");
    int uid = js["uid"].get<int>();
    string pwd = js["password"].get<string>();

    User user = userModel_.query(uid);
    //  用户存在 && 密码正确
    if (user.getUid() == uid && user.getPwd() == pwd)
    {
        //  该用户已登录 不可重复登录
        if (user.getState() == "online")
        {
            json res;
            res["msg_id"] = LOGIN_MSG_ACK;
            res["errno"] = 2;
            res["uid"] = user.getUid();
            res["errmsg"] = "The account has already logged in ; Please enter another account";
            conn->send(res.dump());
        }
        else
        {
            //  登陆成功 记录user连接信息
            //  线程安全
            //  操作多个线程共享对象 上锁
            {
                std::lock_guard<std::mutex> lock(connMtx_);
                userConnTable_.insert({uid, conn});
            }

            //  登录成功 更新用户状态
            user.setState("online");
            userModel_.upState(user);

            //  登录成功信息
            json res;
            res["msg_id"] = LOGIN_MSG_ACK;
            res["errno"] = NONE_ERROR;           // 登陆成功
            res["uid"] = uid;
            res["name"] = user.getName();
            
                //  登录时，加载用户的活动列表
                    //  一个用户的活动存储为一个json 然后dump成string
                    //  所有活动就是vector<string>
                    //  json序列化vector<string> 发送json
            vector<Activity> acs = activityModel_.query(uid);
            vector<string> acs_list;
            for(Activity &ac : acs)
            {
                json js;
                js["aid"] = ac.getAid();
                
                js["uid"] = ac.getUid();
                
                //  cat
                Cat cat = catModel_.queryId(ac.getCid());
                js["cid"] = ac.getCid();
                js["cat_name"] = cat.getName();
                    //  cat category
                    Category cg = categoryModel_.query(cat.getCategoryId());
                    js["category_id"] = cat.getCategoryId();
                    js["cat_category"] = cg.getName();
                    //  cat feature
                    js["feature_id"] = cat.getFeatureId();
                    Feature fea = featureModel_.query(cat.getFeatureId());
                    js["cat_feature"] = fea.getPersonality();

                //  time
                js["time"] = ac.getTime();

                //  activity location
                Location loc = locationModel_.query(ac.getLid());
                js["location_id"] = ac.getLid();
                js["location_region"] = loc.getRegion();
                js["location_specificAddr"] = loc.getSpecificAddr();            //  server 没有设计cache 效率很低
                //  food
                Food f = foodModel_.queryId(ac.getFid());
                js["food_id"] = f.getFid();
                js["food_name"] = f.getName();
                js["food_desc"] = f.getDesc();

                acs_list.push_back(js.dump());
            }
            //  然后把vector<string> acs_list 作为js的一个元素 ，即js["xxx"] = vec（之后再取出，取出的也是vec容器）
            res["activities"] = acs_list;
            //  client接收时，接收到字符串str，先parse反序列化成json
                //  然后一步步逆序拆解
                //  取出每个js["xxx"]
                    //  如果这个js["xxx"] 就是值 那么ok
                    //  如果这个js["xxx"] 不是所需的值 比如是一个容器
                        //  那么就取出容器的元素
                            //  如果元素是str，由json.dump出来的，那么parse成json，然后取出一个个元素
            conn->send(res.dump());
            // {"activities":
            //      [
            //       "{\"aid\":1,\"cat_category\":\"felis_silvestris\",\"cat_feature\":\"grumpy\",\"cat_name\":\"aimi\",\"category_id\":1,\"cid\":1,\"feature_id\":2,\"food_desc\":\"delicious\",\"food_id\":1,\"food_name\":\"apple\",\"location_id\":2,\"location_region\":\"HIT\",\"location_specificAddr\":\"T5\",\"time\":\"0202-12-20 10:01:00\",\"uid\":22}",
            //       "{\"aid\":2,\"cat_category\":\"felis_silvestris\",\"cat_feature\":\"grumpy\",\"cat_name\":\"paimeng\",\"category_id\":1,\"cid\":2,\"feature_id\":2,\"food_desc\":\"delicious\",\"food_id\":3,\"food_name\":\"banana\",\"location_id\":2,\"location_region\":\"HIT\",\"location_specificAddr\":\"T5\",\"time\":\"2022-12-20 10:00:00\",\"uid\":22}"
            //      ]
        }
    }
    else
    {
        //  密码不正确 || 用户不存在 || ChatServer连接 SqlServer失败
        json res;
        res["msg_id"] = LOGIN_MSG_ACK;
        res["errno"] = 2;
        res["uid"] = user.getUid();
        res["errmsg"] = "password is wrong or user doesn't exist or Server internal error";

        cout<<"user send pwd "<<pwd<<endl;
        cout<<"right pwd " <<user.getPwd()<<endl;

        conn->send(res.dump());
    }
}

    

//  处理注册业务
void CatService::reg(const TcpConnectionPtr &conn, json &js,  Timestamp time)
{
    LOG_INFO("do reg service! \n");
    
    if((!js.contains("name")) || (!js.contains("password")))
    {
        LOG_INFO("msg format can not be handled\n");
        return ;
    }

    string name = js["name"];
    string pwd = js["password"];
    User user;
    user.setName(name);
    user.setPwd(pwd);
    
    bool st = userModel_.insert(user);
    if (st)
    {   //  注册成功
        json repo;
        repo["msg_id"] = REG_MSG_ACK;   //  消息类型
        repo["errno"] = NONE_ERROR;              //  0 注册成功
        repo["uid"] = user.getUid();
        repo["errmsg"] = "please remember your id";
        conn->send(repo.dump());        //  发送消息
    }
    else
    {
        //  注册失败
        json repo;
        repo["msg_id"] = REG_MSG_ACK;
        repo["errno"] = REG_ERROR;              //  1 注册失败
        repo["errmsg"] = "server failed to insert data to database";
        conn->send(repo.dump());
    }
}

//  报名活动
void CatService::enroll(const TcpConnectionPtr &conn, json &js, Timestamp time)
{
    LOG_INFO("enroll service");

    int uid = js["uid"].get<int>();
    string c = js["cat"].get<string>();
    // cout<<c<<endl;
    Cat cat = catModel_.queryName(c);            //  cat table的name要加上索引
    string t = js["time"].get<string>();
    // cout<<t<<endl;
    int lid = js["location"].get<int>();
    // cout<<lid<<endl;
    string food_name = js["food"].get<string>();
    // cout<<food_name<<endl;
    //  使用了 food table的index name
    Food food = foodModel_.queryName(food_name);                      

    Activity ac;
    ac.setUid(uid);
    ac.setCid(cat.getCid());
    ac.setTime(t);
    ac.setLid(lid);
    ac.setFid(food.getFid());

    //  注册活动到表上    
    bool res = activityModel_.insert(ac);

    if(!res)
    {
        json repo;
        repo["msg_id"] = ENROLL_MSG_ACK;
        repo["errno"] = ENROLL_ERROR; 
        repo["uid"] = uid;
        repo["errmsg"] = "user failed to enroll";
        conn->send(repo.dump());  
        LOG_INFO("user failed to enroll\n");
        return ;
    }
    
    json repo;
    repo["msg_id"] = ENROLL_MSG_ACK;
    repo["errno"] = NONE_ERROR;
    repo["uid"] = uid;
    repo["aid"] = ac.getAid();
    repo["errmsg"] = "remember your activity id";
    conn->send(repo.dump());

    return ;
}

//  发送给user cat信息表 ；food信息表
void CatService::show(const TcpConnectionPtr &conn, json &jss, Timestamp time)
{
    LOG_INFO("show service");

    json repo;

    vector<Cat> cats = catModel_.queryAll();
    if(!cats.empty())
    {
        vector<string> cat_list;
        for(const auto& cat : cats)
        {
            json js;
            js["cat_name"] = cat.getName();
            //  cat category
            Category cg = categoryModel_.query(cat.getCategoryId());
            js["category_id"] = cat.getCategoryId();
            js["cat_category"] = cg.getName();
            //  cat feature
            js["feature_id"] = cat.getFeatureId();
            Feature fea = featureModel_.query(cat.getFeatureId());
            js["cat_feature"] = fea.getPersonality();

            cat_list.push_back(js.dump());
        }
        repo["cats"] = cat_list;
    }

    vector<Food> foods = foodModel_.queryAll();
    if(!foods.empty())
    {
        vector<string> food_list;
        for(const auto & f : foods)
        {
            json js;
            js["food_id"] = f.getFid();
            js["food_name"] = f.getName();
            js["food_desc"] = f.getDesc();

            food_list.push_back(js.dump());
        }
        repo["foods"] = food_list;
    }

    vector<Location> ls = locationModel_.queryAll();
    if(!ls.empty())
    {
        vector<string> location_list;
        for(const auto & l : ls)
        {
            json js;
            js["location_id"] = l.getLid();
            js["location_region"] = l.getRegion();
            js["location_specificAddr"] = l.getSpecificAddr();

            location_list.push_back(js.dump());
        }
        repo["locations"] = location_list;
    }

    repo["errno"] = NONE_ERROR;
    repo["msg_id"] = SHOW_MSG_ACK;
    repo["errmsg"] = "Here goes the message";

    conn->send(repo.dump());
}

void CatService::cancel(const TcpConnectionPtr &conn, json &js, Timestamp time)
{   
    LOG_INFO("cancel");
    json repo;

    repo["msg_id"] = CANCEL_MSG_ACK;
    bool res = activityModel_.del(js["aid"]);
    if(res)
    {
        repo["errno"] = NONE_ERROR;
        repo["errmsg"] = "successfully cancel the activity";
    }
    else
    {
        repo["errno"] = CANCEL_ERROR;
        repo["errmsg"] = "failed to cancel the activity";
    }

    conn->send(repo.dump());
}

//  服务器终止 重置user表中的所有在线状态
void CatService::reset()
{
    userModel_.resetState();
}

//  更新猫位置
void CatService::updateCat(const TcpConnectionPtr & conn, json & js, Timestamp time)
{
    LOG_INFO("updateCat");
    
    string c = js["cat"].get<string>();
    int lid = js["lid"].get<int>();
    Cat cat = catModel_.queryName(c);
    if(cat.getCid() == -1)
    {
        cat.setName(c);
        catModel_.insert(cat);
    }
    int cid = cat.getCid();

    CatLocation cl(cid,lid);
    //  登记cid出现位置lid
    bool res = catLocationModel_.insert(cl);

    if(!res)
    {
        json repo;
        repo["msg_id"] = UPDATE_MSG_ACK;
        repo["errno"] = UPDATE_ERROR; 
        repo["errmsg"] = "user failed to update";
        conn->send(repo.dump());  
        LOG_INFO("user failed to enroll\n");
        return ;
    }
    
    json repo;
    repo["msg_id"] = UPDATE_MSG_ACK;
    repo["errno"] = NONE_ERROR;
    repo["errmsg"] = "user update successfully";
    conn->send(repo.dump());
}