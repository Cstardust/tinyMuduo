#ifndef _CATEGORY_H_
#define _CATEGORY_H_

#include "public.h"

class Category
{
public:
    Category(int categoryId = -1, const string &name = "", const string& size = "")
        : categoryId_(categoryId), name_(name), size_(size)
    {}

    int getCategorId(){ return categoryId_;}
    string getName() {return name_;}
    string getSize() {return size_;}
    void setCategoryId(int categoryId) {categoryId_ = categoryId;}
    void setName(const string &name){name_ = name;}
    void setSize(const string& size) {size_ = size;}

private:
    int categoryId_;
    string name_; //  种类名称
    string size_; //  大型/小型 : large / small
};

#endif