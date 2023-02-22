#ifndef _FOOD_MODEL_H
#define _FOOD_MODEL_H

#include "Food.hpp"
#include"public.h"
class FoodModel
{
public:
    bool insert(Food &food);
    bool del(int lid);
    Food queryId(int lid);
    Food queryName(const string& name);
    vector<Food> queryAll();
private:
};

#endif