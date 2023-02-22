#ifndef MY_MUDUO_EVENTLOOPTHREAD_H
#define MY_MUDUO_EVENTLOOPTHREAD_H


#include "EventLoopThread.h"
#include<vector>
#include<memory>

class EventLoopThreadPool:noncopyable
{
public:
    using ThreadInitCallback = std::function<void(EventLoop*)> ;
    EventLoopThreadPool(EventLoop *baseLoop = nullptr,const std::string &name = std::string());
    ~EventLoopThreadPool();
    //  开启subReactors
        //  开启底层的numThreads_个thread 且 每个thread上 都创建并运行着一个loop.loop()
    void start(const ThreadInitCallback &cb = ThreadInitCallback());
    void setThreadNum(int num){numThreads_ = num;}
    //  如果工作在多线程中，baseloop_默认以轮询的方式分配channel和subloop
    EventLoop *getNextLoop();
    std::vector<EventLoop*> getAllLoops();    
private:
    EventLoop *baseLoop_;       //  main loop
    std::string name_;
    bool started_;
    int numThreads_;            //  subloop的个数
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> subReactors_;
    std::vector<EventLoop*> loops_;
};

#endif