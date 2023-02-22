#ifndef _FOOD_MODEL_H_
#define _FOOD_MODEL_H_
#include "Feature.hpp"

class FeatureModel
{
public:
    bool insert(Feature &f);
    bool del(int feature_id);
    Feature query(int feature_id);
private:
};


#endif