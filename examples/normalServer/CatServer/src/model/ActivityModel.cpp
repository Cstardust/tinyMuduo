#include "ActivityModel.h"
#include "DB.h"

bool ActivityModel::insert(Activity &activity)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into activity(uid,cid,time,lid,fid) values('%d','%d','%s','%d','%d')",
            activity.getUid(), activity.getCid(), activity.getTime().c_str(), activity.getLid(), activity.getFid());

    MySQLHandler conn;
    if (!conn.connect())
    {
        return false;
    }
    conn.update(sql);
    activity.setAid(mysql_insert_id(conn.getConnection()));
    return true;
}

// Activity ActivityModel::query(int aid)
// {
//     char sql[1024] = {0};
//     sprintf(sql,"select * from activity where aid = %d",aid);

//     MySQLHandler conn;
//     if(!conn.connect())
//     {
//         return Activity();
//     }
//     MYSQL_RES *res = conn.query(sql);      //  mysql_query , mysql_use_query
//     if(res != nullptr)
//     {
//         MYSQL_ROW row = mysql_fetch_row(res);       //  mysql_fetch_row
//         if(row!=nullptr)
//         {
//             return Activity(atoi(row[0]),atoi(row[1]),atoi(row[2]),row[3],row[4],row[5]);
//         }
//     }
// }

//  查询同学uid的所有喂猫活动
vector<Activity> ActivityModel::query(int uid)
{
    char sql[1024] = {0};
    // sprintf(sql, "select u.uid , u.name , c.cid , c.name , a.time , a.location , a.food from activity a inner join user u on u.uid = a.uid inner join cat on u.cid = c.cid where u.uid = %d", uid);
    sprintf(sql, "select * from activity a inner join user u on u.uid = a.uid where u.uid = %d", uid);

    vector<Activity> vec;
    MySQLHandler conn;
    if(!conn.connect())
    {
        LOG_INFO("failed to query uid %d activity\n",uid);
        return vec;
    }

    MYSQL_RES *res = conn.query(sql);
    if(res == nullptr) return vec;
    MYSQL_ROW row;
    while((row = mysql_fetch_row(res))!=nullptr)
    {
        Activity act;
        act.setAid(atoi(row[0]));
        act.setUid(atoi(row[1]));
        act.setCid(atoi(row[2]));
        act.setTime(row[3]);
        act.setLid(atoi(row[4]));
        act.setFid(atoi(row[5]));
        vec.push_back(act);
    }
    return vec;
}


vector<Activity> ActivityModel::queryAll()
{
    // char sql[512] = "select * from h_activity";

    // vector<Activity> vec;
    // MySQLHandler conn;
    // if(!conn.connect())
    // {
    //     LOG_INFO("failed to query activity\n");
    //     return vec;
    // }

    // MYSQL_RES *res = conn.query(sql);
    // if(res == nullptr) return vec;
    // MYSQL_ROW row;
    // while((row = mysql_fetch_row(res))!=nullptr)
    // {
    //     Activity act;
    //     act.setAid(atoi(row[0]));
    //     act.setUid(atoi(row[1]));
    //     act.setCid(atoi(row[2]));
    //     act.setTime(row[3]);
    //     act.setLid(atoi(row[4]));
    //     act.setFid(atoi(row[5]));
    //     vec.push_back(act);
    // }
    // return vec;
    return {};
}


bool ActivityModel::del(int aid)
{
    char sql[1024] = {0};
    sprintf(sql, "delete from activity where aid = %d", aid);

    MySQLHandler conn;
    if (!conn.connect())
    {
        LOG_INFO("failed to delete %d activity\n", aid);
        return false;
    }
    return conn.update(sql);
}