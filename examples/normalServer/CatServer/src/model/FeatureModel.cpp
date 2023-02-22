#include "FeatureModel.h"
#include "DB.h"

bool FeatureModel::insert(Feature &f)
{
    char sql[1024] = {0};
    sprintf(sql,"insert into feature(personality,appearance) values('%s' , '%s')",
        f.getPersonality().c_str(),f.getAppearance().c_str());

    MySQLHandler conn;
    if(!conn.connect())
    {
        return false;
    }
    conn.update(sql);
    f.setFeatureId(mysql_insert_id(conn.getConnection()));
    return true;
}

Feature FeatureModel::query(int feature_id)
{
    char sql[1024] = {0};
    sprintf(sql,"select * from feature where feature_id = %d",feature_id);
    
    MySQLHandler conn;
    if(!conn.connect())
    {
        return Feature();                      //  cid = -1
    }
    MYSQL_RES *res = conn.query(sql);           //  mysql_query , mysql_use_query
    if(res != nullptr)
    {
        MYSQL_ROW row = mysql_fetch_row(res);       //  mysql_fetch_row
        if(row!=nullptr)
        {
            return Feature(atoi(row[0]),row[1],row[2]);
        }
    }
    return Feature();                          //  cid = -1
}



bool FeatureModel::del(int feature_id)
{
    char sql[1024] = {0};
    sprintf(sql,"delete from feature where feature_id = %d",feature_id);

    MySQLHandler conn;
    if(!conn.connect())
    {
        LOG_INFO("failed to delete %d feature\n",feature_id);
        return false;
    }
    return conn.update(sql);
}

