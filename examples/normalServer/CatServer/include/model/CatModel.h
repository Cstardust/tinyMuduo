#ifndef _CAT_MODEL_H_
#define _CAT_MODEL_H_

#include"Cat.hpp"
#include"public.h"

class CatModel
{
public:
    bool insert(Cat &cat);
    bool del(int cid);
    Cat queryName(const string& name);
    Cat queryId(int cid);
    vector<Cat> queryAll();
private:
};


#endif