#include "EventLoopThread.h"
#include "EventLoop.h"
#include "Logger.h"
#include "sys/timerfd.h"
#include "Channel.h"
#include<cstring>
// EventLoopThread::EventLoopThread(const ThreadInitCallback &call /*= ThreadInitCallback()*/,
//                                  const std::string name /*= ""*/)
// :   callback_(call),
//     loop_(new EventLoop()),
//     thread_(std::bind(call,loop_),name),
//     exiting_(false)
// {
// }

// EventLoop * EventLoopThread:: startLoop()
// {
//     thread_.start();
//     return loop_;
// }

// EventLoopThread :: ~EventLoopThread()
// {
//     thread_.join();
//     delete loop_;
// }

// void EventLoopThread::threadFunc()
// {
// }

EventLoopThread::EventLoopThread(const ThreadInitCallback &cb /*= ThreadInitCallback()*/,
                                 const std::string name /*= ""*/)
    :callback_(cb),
    loop_(nullptr),
    thread_(std::bind(&EventLoopThread::threadFunc,this),name),
    exiting_(false),
    mtx_(),
    cond_()
{}


EventLoopThread::~EventLoopThread()
{
    exiting_ = true;
    if(loop_ != nullptr)
    {
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop()
{

    thread_.start();        //  开启thread 跑threadFunc

    {
        //  loop_临界区 因此需要lock
        std::unique_lock<std::mutex> lock(mtx_);
        //  等待thread运行 . 因为这里需要拿到loop的值
        while(loop_ == nullptr)
        {
            cond_.wait(lock);       
            //  1. 当前thread 释放lock锁 
            //  2. 并等待条件满足(即等待被另一个thread通过cond_.notify唤醒正在条件变量上wait的本thread)
            //  3. 当前thread重新获得lock锁 并继续向下运行。如果没有得到锁，会阻塞等待。
        }
    }
    return loop_;
}


//  传给Thread 即将跑在Thread开启的thread上的函数
    //  Eventloop 在开启的thread上创建
void EventLoopThread::threadFunc()
{
    LOG_INFO("main(): pid = %d , tid = %d\n",
            getpid(),CurrentThread::tid());
    // loop_ = new EventLoop();
    EventLoop loop;         //  智能指针不应当被放在堆上 因此不是new EventLoop()，而是local的EventLoop
    
    if(callback_)
    {
        callback_(&loop);
    }

    {
        //  loop_ 是临界区 需要加锁
        std::unique_lock<std::mutex> lock(mtx_);
        loop_ = &loop;
        cond_.notify_one();
    }

    loop.loop();            //  IO线程的loop EventLoop loop -> Poller poll

    //  local loop 出threadFunc即销毁，因此loop_ = nullptr
    std::unique_lock<std::mutex> lock(mtx_);
    loop_ = nullptr;
    LOG_INFO("Thread Finish!\n");
}




        // int timerfd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC);

        // //  创建channel 封装timerfd以及其上的读事件
        // Channel channel(&loop,timerfd);
        // channel.setReadCallback([&](Timestamp t)->void{
        //         LOG_INFO("TIME OUT %s\n",t.now().toString().c_str());
        //         loop.quit();
        //     });
        // //  这个函数会使得fd以及其事件会被注册到epoll上
        // channel.enableReading();

        // struct itimerspec howlong;
        // memset(&howlong,0,sizeof howlong); 
        // howlong.it_value.tv_sec = 5;
        // ::timerfd_settime(timerfd,0,&howlong,nullptr);
