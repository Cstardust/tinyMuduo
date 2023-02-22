#include"CatModel.h"
#include "DB.h"
#include"public.h"

bool CatModel::insert(Cat &cat)
{
    char sql[1024] = {0};
    sprintf(sql,"insert into cat(name,category_id,feature_id) values('%s' , '%d' , '%d')",
        cat.getName().c_str(),cat.getCategoryId(),cat.getFeatureId());

    MySQLHandler conn;
    if(!conn.connect())
    {
        return false;
    }
    //  更新cid
    conn.update(sql);
    cat.setCid(mysql_insert_id(conn.getConnection()));
    return true;
}

Cat CatModel::queryId(int cid)
{
    char sql[1024] = {0};
    sprintf(sql,"select * from cat where cid = %d",cid);
    
    MySQLHandler conn;
    if(!conn.connect())
    {
        return Cat();               //  cid = -1
    }
    MYSQL_RES *res = conn.query(sql);      //  mysql_query , mysql_use_query
    if(res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);       //  mysql_fetch_row
        if(row!=nullptr)
        {
            return Cat(atoi(row[0]),row[1],atoi(row[2]),atoi(row[3]));
        }
    }
    return Cat();                    //  cid = -1
}


Cat CatModel::queryName(const string& name)
{
    char sql[1024] = {0};
    sprintf(sql,"select * from cat where name = '%s' ",name.c_str());
    
    MySQLHandler conn;
    if(!conn.connect())
    {
        return Cat();               //  cid = -1
    }
    MYSQL_RES *res = conn.query(sql);      //  mysql_query , mysql_use_query
    if(res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);       //  mysql_fetch_row
        if(row!=nullptr)
        {
            return Cat(atoi(row[0]),row[1],atoi(row[2]),atoi(row[3]));
        }
    }
    return Cat();                    //  cid = -1
}


bool CatModel::del(int cid)
{
    char sql[1024] = {0};
    sprintf(sql,"delete from cat where cid = %d",cid);

    MySQLHandler conn;
    if(!conn.connect())
    {
        LOG_INFO("failed to delete %d cat\n",cid);
        return false;
    }
    return conn.update(sql);
}

vector<Cat> CatModel::queryAll()
{
    char sql[512] = "select * from cat";

    MySQLHandler conn;
    if(!conn.connect())
    {
        return {};               //  empty
    }
    MYSQL_RES *res = conn.query(sql);
    if(res == nullptr) return {};
    vector<Cat> vec;
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res))!=nullptr)
    {
        Cat cat;
        cat.setCid(atoi(row[0]));
        cat.setName(row[1]);
        cat.setCategoryId(atoi(row[2]));
        cat.setFeatureId(atoi(row[3]));
        vec.push_back(cat);
    }
    return vec;
}