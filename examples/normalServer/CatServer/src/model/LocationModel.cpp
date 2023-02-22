#include "LocationModel.h"
#include "DB.h"
#include<tinyMuduo/Logger.h>

bool LocationModel::insert(Location &location)
{
    char sql[1024] = {0};
    sprintf(sql,"insert into location(region, specific_address) values('%s' , '%s')",
        location.getRegion().c_str(),location.getSpecificAddr().c_str());

    MySQLHandler conn;
    if(!conn.connect())
    {
        return false;
    }
    conn.update(sql);
    location.setLid(mysql_insert_id(conn.getConnection()));
    return true;
}


Location LocationModel::query(int lid)
{
    char sql[1024] = {0};
    sprintf(sql,"select * from location where lid = %d",lid);
    
    MySQLHandler conn;
    if(!conn.connect())
    {
        return Location();               //  cid = -1
    }
    MYSQL_RES *res = conn.query(sql);      //  mysql_query , mysql_use_query
    if(res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);       //  mysql_fetch_row
        if(row!=nullptr)
        {
            return Location(atoi(row[0]),row[1],row[2]);
        }
    }
    return Location();                    //  cid = -1
}



vector<Location> LocationModel::queryAll()
{
    char sql[512] = "select * from location";
    
    MySQLHandler conn;
    if(!conn.connect())
    {
        return {};               //  empty
    }
    MYSQL_RES *res = conn.query(sql);
    if(res == nullptr) 
        return {};
    vector<Location> vec;
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res))!=nullptr)
    {
        Location loc;
        loc.setLid(atoi(row[0]));
        loc.setRegion(row[1]);
        loc.setSpecificAddr(row[2]);
        vec.push_back(loc);
    }
    return vec;
}


bool LocationModel::del(int lid)
{
    char sql[1024] = {0};
    sprintf(sql,"delete from location where lid = %d",lid);

    MySQLHandler conn;
    if(!conn.connect())
    {
        LOG_INFO("failed to delete %d location\n",lid);
        return false;
    }
    return conn.update(sql);
}
