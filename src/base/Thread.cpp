#include "Thread.h"
#include"Logger.h"
#include"CurrentThread.h"
// #include<semaphore.h>
#include<mutex>
#include<condition_variable>

std::atomic<int> Thread:: numCreated_(0);     //  static 变量 类外初始化

Thread::Thread(const ThreadFunc& func,const std::string & name /*= ""*/)
        : started_(false),
        joined_(false),
        thread_(nullptr),
        tid_(0),
        func_(std::move(func)), 
        name_(name)
    {
        setDefaultName();
    }


Thread::~Thread()
{   
    //  要么join等待回收线程
    //  要么线程分离
    if(started_ && !joined_)
    {
        LOG_INFO("thread : name = %s , tid = %d is detached\n",
            name().c_str(),tid());
        thread_->detach();      //  线程分离
    }
}


void Thread::setDefaultName()
{
    ++numCreated_;              //  atmoic ++
    if(name_.empty())
    {
        char buf[16]={0};
        sprintf(buf,"thread %d",numCreated_.load());
        name_ = buf;    
    }
}


void Thread::start()
{
    std::mutex mtx;
    std::condition_variable cond;
    //  start thread
    thread_ = std::unique_ptr<std::thread>(new std::thread([&](void)->void{

        //  临界区: tid_ , thread func内要写;开启thread的thread要读。
        mtx.lock();
        tid_ = CurrentThread::tid();
        cond.notify_one();
        mtx.unlock();

        //  threadFunc
        func_();
    }));

    {
        std::unique_lock<std::mutex> lock(mtx);
        while(!(tid_ != 0)){
            cond.wait(lock);
        }
    }
    started_ = true;
}

void Thread::join()
{
    //  阻塞等待thread死亡
    joined_ = true;
    thread_->join();
}


