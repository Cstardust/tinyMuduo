#include "FoodModel.h"
#include "DB.h"

bool FoodModel::insert(Food &food)
{
    char sql[1024] = {0};
    sprintf(sql,"insert into food(name,description) values('%s' , '%s')",
        food.getName().c_str(),food.getDesc().c_str());

    MySQLHandler conn;
    if(!conn.connect())
    {
        return false;
    }
    conn.update(sql);
    food.setFid(mysql_insert_id(conn.getConnection()));
    return true;
}

Food FoodModel::queryId(int fid)
{
    char sql[1024] = {0};
    sprintf(sql,"select * from food where fid = %d",fid);
    
    MySQLHandler conn;
    if(!conn.connect())
    {
        return Food();                      //  cid = -1
    }
    MYSQL_RES *res = conn.query(sql);      //  mysql_query , mysql_use_query
    if(res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);       //  mysql_fetch_row
        if(row!=nullptr)
        {
            return Food(atoi(row[0]),row[1],row[2]);
        }
    }
    return Food();                          //  cid = -1
}

Food FoodModel::queryName(const string& name)
{
    char sql[1024] = {0};
    sprintf(sql,"select * from food where name = '%s' ",name.c_str());
    
    MySQLHandler conn;
    if(!conn.connect())
    {
        return Food();                      //  cid = -1
    }
    MYSQL_RES *res = conn.query(sql);      //  mysql_query , mysql_use_query
    if(res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);       //  mysql_fetch_row
        if(row!=nullptr)
        {
            return Food(atoi(row[0]),row[1],row[2]);
        }
    }
    return Food();                          //  cid = -1
}


bool FoodModel::del(int fid)
{
    char sql[1024] = {0};
    sprintf(sql,"delete from food where fid = %d",fid);

    MySQLHandler conn;
    if(!conn.connect())
    {
        LOG_INFO("failed to delete %d food\n",fid);
        return false;
    }
    return conn.update(sql);
}

vector<Food> FoodModel::queryAll()
{
    char sql[512] = "select * from food";

    MySQLHandler conn;
    if(!conn.connect())
    {
        return {};               //  empty
    }
    MYSQL_RES *res = conn.query(sql);
    if(res == nullptr) return {};
    vector<Food> vec;
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res))!=nullptr)
    {
        Food food;
        food.setFid(atoi(row[0]));
        food.setName(row[1]);
        food.setDesc(row[2]);
        vec.push_back(food);
    }
    return vec;
}