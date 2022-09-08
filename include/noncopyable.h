#ifndef MY_MUDUO_NONCOPYABLE_H

#define MY_MUDUO_NONCOPYABLE_H


/*
 * noncopyable的派生类
 *      可以正常构造和析构，但是不可以拷贝构造和赋值
 *          子类构造、析构、拷贝都会调用到parent的相应函数
 *
 * 
 */

class noncopybale
{
public:
    noncopybale (const noncopybale&) = delete;
    noncopybale& operator=(const noncopybale&) = delete;
protected:
    noncopybale()=default;
    ~noncopybale()=default;
};



#endif