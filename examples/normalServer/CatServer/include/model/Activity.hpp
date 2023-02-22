#ifndef _ACTIVITY_H_
#define _ACTIVITY_H_

#include "public.h"

class Activity
{
public:
    Activity(int aid = -1, int uid = -1, int cid = -1, const string &time = "", int lid = -1,int fid = -1)
        : aid_(aid), uid_(uid), cid_(cid), time_(time), lid_(lid), fid_(fid)
    {
    }
    int getAid() { return aid_; }
    int getUid() { return uid_; }
    int getCid() { return cid_; }
    string getTime() { return time_; }
    int getLid() { return lid_; }
    int getFid() { return fid_; }

    void setAid(int aid) {assert(aid_ == -1); aid_ = aid;}
    void setUid(int uid) { uid_ = uid; }
    void setCid(int cid) { cid_ = cid; }
    void setTime(const string&time) { time_ = time; }
    void setLid(int lid) { lid_ = lid; }
    void setFid(int fid) { fid_ = fid; }

private:
    int aid_;
    int uid_;
    int cid_;
    string time_;
    int lid_;        //  location_id
    int fid_;        //  food_id
};

#endif