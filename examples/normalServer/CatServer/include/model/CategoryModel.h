#ifndef _CATEGORY_MODEL_H_
#define _CATEGORY_MODEL_H_

#include "Category.hpp"

class CategoryModel
{
public:
    bool insert(Category &ct);
    bool del(int cate_id);
    Category query(int cate_id);
private:
};



#endif