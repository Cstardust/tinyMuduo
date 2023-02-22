#include"DB.h"
#include"UserModel.h"
#include "User.hpp"
#include<tinyMuduo/Logger.h>


bool UserModel::insert(User &user)
{
    char sql[1024] = {0};
    sprintf(sql,"insert into user(name,password,state) values('%s','%s' , '%s')",
        user.getName().c_str(),user.getPwd().c_str(),user.getState().c_str());

    MySQLHandler conn;
    if(!conn.connect())
    {
        return false;
    }
    if(!conn.update(sql))
        return false;
    user.setUid(mysql_insert_id(conn.getConnection()));
    return true;
}


//  根据id返回User
User UserModel::query(int id)
{
    char sql[1024] = {0};
    sprintf(sql,"select * from user where uid = %d",id);
    
    MySQLHandler conn;
    if(!conn.connect())
    {
        return User();      //  default id = -1 返回User'id = -1 代表 用户没注册过 或者 连接sql server失败
    }
    MYSQL_RES *res = conn.query(sql);      //  mysql_query , mysql_use_query
    if(res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);       //  mysql_fetch_row
        if(row!=nullptr)
        {
            return User(atoi(row[0]),row[1],row[2],row[3]);
        }
    }
    return User();          //  uid = -1
}


bool UserModel::del(int uid)
{
    char sql[1024] = {0};
    sprintf(sql,"delete from user where uid = %d",uid);

    MySQLHandler conn;
    if(!conn.connect())
    {
        LOG_INFO("failed to delete %d user\n",uid);
        return false;
    }
    return conn.update(sql);
}

bool UserModel::upState(User &user)
{
    char sql[1024]={0};
    sprintf(sql,"update user set state = '%s' where uid = %d",user.getState().c_str(),user.getUid());

    MySQLHandler mysql_handler;
    if(mysql_handler.connect())
    {
        if(mysql_handler.update(sql))
        {
            return true;
        }
    }
    return false;
}

void UserModel::resetState()
{
    const char *sql = " update user set state = 'offline' where state = 'online' ";
    MySQLHandler mysql_handler;
    if(mysql_handler.connect())
    {
        mysql_handler.update(sql);
    }
}   