#include "CategoryModel.h"
#include "DB.h"

bool CategoryModel::insert(Category &ct)
{
    char sql[1024] = {0};
    sprintf(sql,"insert into category(name,size) values('%s' , '%s')",
        ct.getName().c_str(),ct.getSize().c_str());

    MySQLHandler conn;
    if(!conn.connect())
    {
        return false;
    }
    conn.update(sql);
    ct.setCategoryId(mysql_insert_id(conn.getConnection()));
    return true;
}

Category CategoryModel::query(int category_id)
{
    char sql[1024] = {0};
    sprintf(sql,"select * from category where category_id = %d",category_id);
    
    MySQLHandler conn;
    if(!conn.connect())
    {
        return Category();                      //  cid = -1
    }
    MYSQL_RES *res = conn.query(sql);           //  mysql_query , mysql_use_query
    if(res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);       //  mysql_fetch_row
        if(row!=nullptr)
        {
            return Category(atoi(row[0]),row[1],row[2]);
        }
    }
    return Category();                          //  cid = -1
}



bool CategoryModel::del(int category_id)
{
    char sql[1024] = {0};
    sprintf(sql,"delete from category where category_id = %d",category_id);

    MySQLHandler conn;
    if(!conn.connect())
    {
        LOG_INFO("failed to delete %d category\n",category_id);
        return false;
    }
    return conn.update(sql);
}

