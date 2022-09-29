#include "EventLoopThreadPool.h"
#include "Logger.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop *baseloop, const std::string &name)
    : baseLoop_(baseloop),
      name_(name),
      started_(false),
      numThreads_(0),
      next_(0)
{
}


EventLoopThreadPool::~EventLoopThreadPool()
{
    //  no need to delete loop
    //  it's a stack variable
}


//  threadinitcallback有什么用？
void EventLoopThreadPool::start(const ThreadInitCallback& cb/* = ThreadInitCallback()*/)
{
    started_ = true;

    for(int i=0;i<numThreads_;++i)
    {
        char buf[name_.size() + 32];
        snprintf(buf,sizeof buf,"%s%d",name_.c_str(),i);
        //  new EventLoopthread which has loop_ and will start a thread
        EventLoopThread *t = new EventLoopThread(cb,buf);
        //  EventLoopThread 交给unique_ptr管理 负责销毁
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        //  EventLoopThread对象底层创建一thread 该thread会调用底层的threadFunc 并进入loop循环。
        //  loops中的EventLoop* 是EventLoopThread创建的local variable
        //  因此 并不需要delete loop 更不需要在eventloopthreadpool这层delete loop
        loops_.push_back(t->startLoop());
    }

    //  整个Server端中只有一个thread，运行着baseloop
    if(numThreads_ == 0 && cb)
    {
        cb(baseLoop_);
    }
}

//  io thread : baseloop
//  work thread : threads
//  如果没有对EventLoop设置个数的话
    //  即，如果numThread = 0的话，
    //  那么就只有一个main thread ,跑一个baseloop
    //  即 在那个baseloop所在的main thread 既做IO又做worker
//  当然 muduo是个multiple reactor
    //  应当是多线程


//  如果工作在多线程中，baseloop默认以轮询的方式分配channel给subloop
EventLoop *EventLoopThreadPool::getNextLoop()
{
    //  loops为空 即ThreadPool并没开启任何thread
        //   只有一个thread 那就是main thread ，也即，调用本Pool的thread
        //   那么就返回baseloop_
    if(loops_.empty()){
        return baseLoop_;
    }

    //  loops不为空 取出下一个loop 并返回
    EventLoop* loop = loops_[next_++];
    next_ %= loops_.size();
    return loop;
}

