#include"CatLocationModel.h"
#include "DB.h"

bool CatLocationModel::insert(CatLocation &ct)
{
    char sql[1024] = {0};
    sprintf(sql,"insert into cat_location(cid,lid) values(%d ,%d)",
        ct.getCid(),ct.getLid());

    MySQLHandler conn;
    if(!conn.connect())
    {
        return false;
    }
    return conn.update(sql);
}


bool CatLocationModel::del(int id)
{
    LOG_INFO("has not finished!\n");
    return false;
}

CatLocation CatLocationModel::query(int cate_id)
{
    LOG_INFO("has not finished!\n");
    return {};
}