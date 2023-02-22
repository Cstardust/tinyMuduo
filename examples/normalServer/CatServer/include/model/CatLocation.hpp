#ifndef _CatLocation_H_
#define _CatLocation_H_

#include "public.h"

class CatLocation
{
public:
    CatLocation(int cid = -1,int lid = -1)
        : cid_(cid),lid_(lid)
    {}
    
    int getCid() { return cid_; }
    int getLid() { return lid_; }

    void setCid(int cid) { cid_ = cid; }
    void setLid(int lid) { lid_ = lid; }

private:
    int cid_;        //  cat_id
    int lid_;        //  location_id
};

#endif