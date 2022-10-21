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

//  创建并启动一个thread
EventLoop* EventLoopThread::startLoop()
{

    thread_.start();      
    //  loop_临界区 : 开启的thread要写。开启thread的main thread 要读。
    {
        std::unique_lock<std::mutex> lock(mtx_);
        while(!(loop_ != nullptr))          //  ! cond
        {
            cond_.wait(lock);       
        }
    }
    return loop_;
}

//  即将跑在subThread的threadFunc: loop.loop()
void EventLoopThread::threadFunc()
{
    LOG_INFO("main(): pid = %d , tid = %d\n",
            getpid(),CurrentThread::tid());

    EventLoop loop;         
    
    if(callback_)
    {
        callback_(&loop);
    }

    {
        std::unique_lock<std::mutex> lock(mtx_);
        loop_ = &loop;              
        LOG_INFO("loop_ = %p in thread %d",&loop,CurrentThread::gettid());
        cond_.notify_one();
    }

    loop.loop();          

    //  local loop 出threadFunc即销毁，因此loop_ = nullptr
    std::unique_lock<std::mutex> lock(mtx_);
    loop_ = nullptr;
    LOG_INFO("Thread Finish!\n");
}


