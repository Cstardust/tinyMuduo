#ifndef _LOCATION_MODEL_H_
#define _LOCATION_MODEL_H_

#include"Location.hpp"

class LocationModel
{
public:
    bool insert(Location &l);
    bool del(int lid);
    Location query(int lid);
    vector<Location> queryAll();
};


#endif