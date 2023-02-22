#ifndef _LOCATION_H_
#define _LOCATION_H_

#include "public.h"

class Location
{
public:
    Location(int lid = -1, const string &region = "", const string &specificAddr = "") : lid_(lid), region_(region), specificAddr_(specificAddr)
    {
    }
    int getLid() const { return lid_; }
    string getRegion() const { return region_; }
    string getSpecificAddr() const { return specificAddr_; }

    void setLid(int lid) { lid_ = lid; }
    void setRegion(const string &reg) { region_ = reg; }
    void setSpecificAddr(const string &addr) { specificAddr_ = addr; }

private:
    int lid_;
    string region_;
    string specificAddr_;
};

#endif