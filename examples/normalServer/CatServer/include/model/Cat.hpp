#ifndef _CAT_H_
#define _CAT_H_

#include "DB.h"
#include <cassert>

//  cat的orm类
class Cat
{
public:
    Cat(int cid = -1, const string &name = "", int categoryId = -1, int featureId = -1)
        : cid_(cid), name_(name), categoryId_(categoryId), featureId_(featureId)
    {
    }

    int getCid() const { return cid_; }
    string getName() const { return name_; }
    int getCategoryId() const { return categoryId_; }
    int getFeatureId() const { return featureId_;}

    void setCid(int cid){ assert(cid_ == -1); cid_ = cid; }
    void setName(const string &name) {name_ = name;}
    void setCategoryId(const int& categoryId) { categoryId_ = categoryId; }
    void setFeatureId(int featureId) {featureId_ = featureId;}
private:
    int cid_;
    string name_;
    int categoryId_;
    int featureId_;
};

#endif