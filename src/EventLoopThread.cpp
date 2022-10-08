#include "EventLoopThread.h"
#include "EventLoop.h"
#include "Logger.h"
#include "sys/timerfd.h"
#include "Channel.h"
#include<cstring>


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

//  创建并启动一个thread 并且在该thread上创建一个EventLoop对象（threadFunc运行在该thread上）。
    //  这个启动的thread上 调用 loop.loop()
EventLoop* EventLoopThread::startLoop()
{

    thread_.start();        //  开启thread 跑threadFunc
    //  loop_临界区原因:
        //  为了startLoop之后 loop_指针一定不是null,一定指向本subThread中的subLoop
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


//  即将跑在subThread的threadFunc
//  创建subLoop的Eventloop对象 调用其loop()
void EventLoopThread::threadFunc()
{
    LOG_INFO("main(): pid = %d , tid = %d\n",
            getpid(),CurrentThread::tid());

    EventLoop loop;         
    
    if(callback_)
    {
        callback_(&loop);
    }

    //  感觉没必要枷锁。。。
    {
        //  loop_ 是临界区 需要加锁
        std::unique_lock<std::mutex> lock(mtx_);
        loop_ = &loop;              //  ????
        LOG_INFO("loop_ = %p in thread %d",&loop,CurrentThread::gettid());
        cond_.notify_one();
    }

    loop.loop();            //  IO线程的loop EventLoop loop -> Poller poll

    //  local loop 出threadFunc即销毁，因此loop_ = nullptr
    std::unique_lock<std::mutex> lock(mtx_);
    loop_ = nullptr;
    LOG_INFO("Thread Finish!\n");
}


