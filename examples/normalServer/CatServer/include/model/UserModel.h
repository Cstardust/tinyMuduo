#ifndef _USER_MODEL_H_
#define _USER_MODEL_H_

#include "DB.h"
#include "User.hpp"

//  用于操作User表的工具类
//          insert
//          delete
//          query
class UserModel
{
public:
    bool insert(User& user);
    User query(int uid);
    bool del(int uid);
    bool upState(User &user);
    void resetState();
private:
};

#endif