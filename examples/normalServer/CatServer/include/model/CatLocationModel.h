#ifndef _CATLOCATION_MODEL_H_
#define _CATLOCATION_MODEL_H_

#include "CatLocation.hpp"

class CatLocationModel
{
public:
    bool insert(CatLocation &cl);
    bool del(int id);
    CatLocation query(int cate_id);
private:
};



#endif