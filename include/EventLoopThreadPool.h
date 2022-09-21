#ifndef MY_MUDUO_EVENTLOOPTHREAD_H
#define MY_MUDUO_EVENTLOOPTHREAD_H


#include "EventLoopThread.h"
#include<vector>
#include<memory>

class EventLoopThreadPool:noncopybale
{
public:
    using ThreadInitCallback = std::function<void(EventLoop*)> ;
    EventLoopThreadPool(EventLoop *baseLoop = nullptr,const std::string &name = std::string());
    ~EventLoopThreadPool();
    //  开启线程？
    void start(const ThreadInitCallback &cb = ThreadInitCallback());

    void setThreadNum(int num){numThreads_ = num;}

    //  如果工作在多线程中，baseloop_默认以轮询的方式分配channel和subloop
    EventLoop *getNextLoop();
    std::vector<EventLoop*> getAllLoops();    

private:
    EventLoop *baseLoop_;       //  IO线程？EventLoop loop
    std::string name_;
    bool started_;
    int numThreads_;            //  subloop的个数
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};

#endif