#include "Poller.h"
#include "EventLoop.h"
#include "CurrentThread.h"
#include "Channel.h"
#include"Logger.h"
#include<sys/eventfd.h>
#include<cassert>
#include<sys/poll.h>

//  one loop per thread
//  我们只允许一个线程运行一个EventLoop对象的loop()，直到前一个EventLoop对象销毁，才允许创建下一个。
//  防止一个线程创建多个EventLoop
thread_local EventLoop *t_loopInThisThread = nullptr;

//  定义默认的Poller IO复用接口的超时时间
const int kPollerTimeMs = 10000;

// ???
//  创建wakeupFd,用来notify唤醒subReactor处理新来的Channel
int createEventfd()
{
    int evtfd = ::eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC);  // ????EFD_NONBLOCK??
    if(evtfd < 0)
    {
        LOG_FATAL("eventfd error:%d\n",errno);
    }
    return evtfd;
}


EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      threadId_(CurrentThread::tid()),
      poller_(Poller::newDefaultPoller(this)),
      wakeupFd_(createEventfd()),
      wakeupChannel_(new Channel(this,wakeupFd_)),
      currentActiveChannel_(nullptr),
      callingPendingFactors_(false),
      mtx_()
{
    LOG_INFO("EventLoop created %p in thread %d\n",this,threadId_);
    if(t_loopInThisThread != nullptr)
    {
        LOG_FATAL("Another EventLoop %p is existed in this thread %d\n",getEventOfCurrentThread(),threadId_);
    }
    else
    {
        //  初始化该线程的t_loopInThisThread
        t_loopInThisThread = this;
    }
    
    //  设置wakeupFd的事件类型 以及发生事件后的回调操作
        //  每个EventLoop都监听wakeupFd读事件 
    //  当一个Channel调用enable/disbale时，这个channel就会将自己绑定到其所属的EventLoop上的poller
    //  此时就会将wakeupChannel的wakeupFd_以及相关events注册给poller
    
    //  IO线程监听wakeupFd_(eventfd) 
    //  当有数据写入eventfd时，epoll_wait会监听到这个读事件 于是IO线程就可以从epoll_wait的阻塞中离开
    //  然后IO线程处理这个eventfd的读事件，调用之前注册的handleRead 将数据读出，然而并没有做什么其他的。可能仅仅就是为了有个hanle，判断下eventFd read是否发生错误
    wakeupChannel_->enableReading();       
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleRead,this,std::placeholders::_1));

    //  问题？
    //  这是创建eventfd的线程       IO线程
    //  那么worker线程呢？上面跑的函数是什么？不会也是eventloop的loop函数吧？？
    //  是哪个线程写入eventfd，唤醒IO线程？？
}



// loop
// loop -> epoll_wait -> handleEvent(IO线程监听到的event) -> doPendingFunctor()(其他worker线程加入的event cb)
//          epoll_wait 可能被eventfd唤醒 为了 quit 或者执行其他worker线程加入的cb doPendingFunctor()
//  EventLoop核心
//  只有创建了EventLoop Object的线程，才有资格调用该object的loop方法。也即，才有资格成为mainReactor,才有资格成为IO线程，接收连接。
    //  通过assertInloopThread保证这点
void EventLoop::loop()
{
    assert(!looping_);
    
    assertInLoopThread();

    looping_ = true;
    quit_ = false;

    LOG_INFO("EventLoop %p belonged to thread %d start looping on thread %d\n",this,threadId_,CurrentThread::tid());

    while(!quit_)
    {
        LOG_INFO("TEST\n");
        activeChannels_.clear();
        //  重点！！！
        //  获取Poller监听的发生的事件
        //  监听两类fd
            //  一种是 clientfd
            //  一种是 **wakeupfd**
        pollReturnTime_ = poller_->poll(kPollerTimeMs,&activeChannels_);
        
        //  轮询处理事件
            //  这不就成了在IO线程 既处理accept事件，又处理read/write事件？
            //  IO线程难道不是只应该负责接收client发起的连接事件？
            //  然后将读写事件派发给其他线程完成??
        for(Channel *channel:activeChannels_)
        {
            channel->isReading();
            LOG_INFO("%lu IN HANDLING %d\n",activeChannels_.size(),channel->fd());
            //  poller能够监听哪些channel发生事件，然后上报给EventLoop，
            //  EventLoop通知channel处理相应事件
            channel->handleEvent(pollReturnTime_);
            //  猜测
            //  感觉像是 在这些注册handle函数里，会开启其他线程，来执行work
            //  这些在hanle里开启的线程 就是worker线程。
        }

        //  执行当前EventLoop事件循环需要处理的回调操作
        /*
        * mainReactor IO 线程 accept fd=>channel  --分发给-->subloop
        * mainloop 拿到新channel之后 分发给subloop 
        * mainloop 实现注册一个回调cb。 这个cb需要由subloop来执行
        * mainloop wakeup以后 将subloop唤醒 执行mainloop注册的回调cb
        */
        doPendingFunctors(); 
    }

    looping_ = false;

    LOG_INFO("EventLoop %p belonged to thread %d stop looping on thread %d\n",this,threadId_,CurrentThread::tid());

}


//  退出事件循环
    //  在loop所属的线程(IO线程)中，loop调用quit()。
        //  那么，只需要修改quit_ = true;。当loop的while下一次while(!quit)时，就会生效quit
    //  在非loop所属的线程(IO线程)中，loop调用quit()。
        //  那么，只修改quit_ 的话，会有长达数秒的延迟。
        //  因为当前线程不是IO线程，需要切回IO线程，再执行到while的下一次while(!quit)，才会生效quit
    //  在非loop的线程中 调用loop的quit（怎么做到的？？怎么不在自己的线程 调用自己的函数?） 
        //  那个线程拿到loop指针就可以做到了呗。
void EventLoop::quit()
{
    LOG_INFO("tid = %d call EventLoop quit\n",CurrentThread::tid());
    //  loop 的 while会随之退出
    quit_ = true;       
    //  如果在其他线程中调用的quit。比如在一个subloop(worker)中，调用了mainloop的quit
    if(!isInLoopThread())
    {
        wakeup();               //  通过向eventfd写入数据 使得监听eventfd的IO线程从epoll_wait中返回 脱离阻塞状态
    }

    //  通过轮询直接派发 如何唤醒？通过eventfd 
    //  并不是IO线程和worker线程中间有一个缓冲队列。然后mainloop拿到channel之后放入队列 然后所有的worker从缓冲队列里拿channel进行消费
}




//  ? 唤醒谁的 ? wakeupFd 发生的事件是只能唤醒IO线程 还是可以唤醒IO线程+worker线程？工程运行时到底有几个loop？
//  mainReactor 唤醒 subReactor
//  subReactor处理wakeupFd的读事件
void EventLoop::handleRead(const Timestamp& )
{
    LOG_INFO("handle read\n");
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_,&one,sizeof one);
    if(n!=sizeof one)
    {
        LOG_ERROR("EventLoop::handleRead() reads %ld bytes instead of 8\n",n);
    }
}



//  runInLoop -> IO cb()
//            -> Worker queueInLoop(cb)->wakeup->IO cb()  
//  猜测 作用：
//  让loop对象执行回调函数cb
    //  如果 调用loop.runInloop(cb)的线程 正好是loop对象所属的线程(IO线程)
        //  那么 直接执行cb
    //  如果 调用loop.runInloop(cb)的线程 不是loop对象所属的线程(IO线程)
        //  那么 将cb存入loop所属的IO线程的回调队列。
        //  loop对象会待会在IO线程调用这些回调cb
//  cb作用：执行Event Handler 开启worker线程??????
void EventLoop::runInLoop(const Functor& cb)
{
    //  如果 当前正在运行的线程是创建EventLoop的线程(IO线程)
        //  那么，直接调用回调函数
    //  也即，如果用户在当前IO线程调用这个函数，回调会同步进行
    if(isInLoopThread())
    {
        cb();
    }
    //  如果 用户在非IO线程，也即非创建该EventLoop Object的线程，调用了该EventLoop Object的runInLoop函数
        //  那么，回调cb会被加入队列，IO线程会被唤醒来调用这个cb回调
    else
    {
        queueInLoop(cb);
    }
}


//  真tm难
//  事件循环四：23:10
//  把cb放入队列 唤醒loop所在线程 执行cb
void EventLoop::queueInLoop(const Functor& cb)
{
    //  给loop添加新回调
    {
        std::lock_guard<std::mutex> lock(mtx_);
        pendingFunctors_.emplace_back(cb);
    }

    //  下面这段还难以理解
    //  只有在IO线程的事件回调中 调用queueInLoop() 才无须wakeup()!
    //  唤醒相应的，需要执行上面回调操作的loop的线程。
        //  callingPendingFactor = true 代表当前正在执行回调 没有阻塞在loop上 , 但是loop又有了新回调 ???
    if(!isInLoopThread() || callingPendingFactors_)       //  这里的callingPendingFactors待解释  
    {
        wakeup();           //  唤醒loop所在线程。通过eventfd唤醒。？？哪个线程？哪个loop?
    }
}



//  用来唤醒loop所在的(所属于的)线程(IO线程)。向wakeupFd_写一个数据。
//  wakeupChannel就发生读事件 当前loop线程就会被唤醒。
void EventLoop::wakeup()
{
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_,&one,sizeof one);
    if(n != sizeof one)
    {
        LOG_ERROR("EventLoop:wake() writes %lu bytes instead of 8\n",n);
    }
}


//  local vector和类的functor的交换
    //  减小了临界区的长度，使得doPendingFunctor 和 queueInLoop可以并发执行
        //  因为只有在对pendingFunctors操作时需要上锁 时间很短
        //  这个doPendingFunctor函数是在functors上操作，而非类内全局变量pendingFunctors
    //  也避免了dead lock 
        //  因为Functor回调函数 可能会调用queueInLoop . queueInLoop需要上锁。
void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFactors_ = true;      //  loop是否有需要执行的回调操作
    {
        std::lock_guard<std::mutex> lock(mtx_);
        functors.swap(pendingFunctors_); //  交换。functor获取所有需要执行的cb。pendingFunctors置空
    }
    for(const Functor &functor : functors)
    {
        functor();                      //  执行当前loop需要执行的回调操作
        //  在这些functor中 会开启一个个worker线程 去处理业务? 
    }

    callingPendingFactors_ = false;
}



void EventLoop::updateChannel(Channel *channel)
{
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel* channel)
{
    return poller_->hasChannel(channel);
}


//  回收资源
EventLoop::~EventLoop()
{
    wakeupChannel_->disableAll();       //  取消wakeupfd感兴趣的事件
    wakeupChannel_->remove();           //  从poller中删除channel
    ::close(wakeupFd_);
    assert(!looping_);
    t_loopInThisThread = nullptr;       
}




EventLoop *EventLoop::getEventOfCurrentThread()
{
    return t_loopInThisThread;
}

void EventLoop::assertInLoopThread()
{
    if(!isInLoopThread())       //  threadId_ == CurrentThread::tid()
    {
        abortNotInLoopThread(); //  
    }
}
void EventLoop::abortNotInLoopThread()
{
  LOG_FATAL("EventLoop::abortNotInLoopThread - EventLoop %p  was created in threadId_ = %d , current thread id = %d\n",this,threadId_,CurrentThread::tid());
}