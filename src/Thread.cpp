#include "Thread.h"
#include"Logger.h"
#include"CurrentThread.h"
#include<semaphore.h>

std::atomic<int> Thread:: numCreated_(0);     //  static 变量 类外初始化

Thread::Thread(const ThreadFunc& func,const std::string & name /*= ""*/)
        : started_(false),
        joined_(false),
        thread_(nullptr),
        tid_(0),
        func_(std::move(func)), //  const & -> && ? 可行吗
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


//  Thread 类对象 : 保存了该对象开启的thread的信息。如tid
void Thread::start()
{
//  sempahore 
    //  我们无法保证 开启的thread和这个start谁先运行
    //  因此 需要有一个机制 来保证 start结束之后 user可以通过Thread对象安全的访问新生的thread的信息 如tid
    //  因此 就用了信号量
    sem_t sem;
    sem_init(&sem,false,0);
    //  开启线程
        //  [&] lambda以引用的方式捕获外界值
    // thread_.reset(new std::thread(func_));   //  开启线程 （给shared_ptr赋值）
    thread_ = std::unique_ptr<std::thread>(new std::thread([&](void)->void{
        //  获取线程的tid值 存入Thread的tid_变量
        tid_ = CurrentThread::tid();
        //  ++sem
        sem_post(&sem);
        //  该线程的任务 专门执行func_()
        func_();
    }));
    //  等待被信号量唤醒
    sem_wait(&sem);
    started_ = true;
}

void Thread::join()
{
    //  阻塞等待thread死亡
    joined_ = true;
    thread_->join();
}


