#ifndef MY_MUDUO_EVENTLOOP_THREAD
#define MY_MUDUO_EVENTLOOP_THREAD

#include "noncopyable.h"
#include "Thread.h"
#include<functional>
#include<string>
#include<mutex>
#include<condition_variable>

class EventLoop;

//  将EventLoop和Thread结合
    //  实现 one loop per thread
class EventLoopThread:noncopyable
{
public:
    //  fucntion 定义函数对象类型时 <>加入返回参数以及传入参数
    //  定义 function所定义的函数对象类型的 函数对象时 可以用std::bind。将多余的参数绑定到原本的函数对象，只留下std::function定义的函数对象所需要的参数类型的空位
    using ThreadInitCallback = std::function<void(EventLoop*)>;
public:
    EventLoopThread(const ThreadInitCallback & call=ThreadInitCallback(),const std::string name = "");
    ~EventLoopThread();
    //  thread_开启循环
    EventLoop *startLoop();
private:
    void threadFunc();
private:
    ThreadInitCallback callback_;
    EventLoop *loop_;           //  EventLoop + 
    Thread thread_;             //  Thread = one loop per thread
    bool exiting_;
    std::mutex mtx_;
    std::condition_variable cond_;
};


#endif
