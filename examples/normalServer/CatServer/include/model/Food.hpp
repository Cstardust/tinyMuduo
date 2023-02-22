#ifndef _FOOD_H_
#define _FOOD_H_

#include "public.h"

class Food
{
public:
    Food(int fid = -1,const string &name = "",const string &desc = "")
        :fid_(fid),name_(name),desc_(desc)
    {}
    int getFid() const {return fid_;}
    string getName() const {return name_;}
    string getDesc() const {return desc_;}
    void setFid(int fid){fid_ = fid;}
    void setName(const string &name){name_ = name;}
    void setDesc(const string &desc){desc_ = desc;}
private:
    int fid_;
    string name_;
    string desc_;    
};


#endif