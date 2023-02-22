#ifndef _ACTIVITY_MODEL_H_
#define _ACTIVITY_MODEL_H_

#include "Activity.hpp"
#include<vector>

using std::vector;

class ActivityModel
{
public:
    bool insert(Activity &activity);
    bool del(int aid);
    vector<Activity> query(int uid);
    vector<Activity> queryAll();
private:
};

#endif