#ifndef _USER_H_
#define _USER_H_

#include"public.h"
#include<cassert>

//  user的orm类
class User
{
public:
    User(int uid = -1 , const string& name = "" , const string& pwd = "", const string &state="offline")
        :uid_(uid) , name_(name) , pwd_(pwd) , state_(state)
    {}

    int getUid() const
    {
        return uid_;
    }

    string getName() const
    {
        return name_;
    }

    string getPwd() const
    {
        return pwd_;
    }

    string getState() const
    {
        return state_;
    }

    void setUid(int uid)
    {
        assert(uid_ == -1);
        uid_ = uid;
    }

    void setName(const string&name) //  can not use name any more
    {
        name_ = name;        
    }

    void setPwd(const string&pwd)
    {
        pwd_ = pwd;
    }

    void setState(const string&st)
    {
        state_ = st;
    }
private:
    int uid_;           //  primary key 、auto_increment
    string name_;       //  not null 、 unique
    string pwd_;        //  not null
    string state_;      //  online / offline(default)
};

#endif