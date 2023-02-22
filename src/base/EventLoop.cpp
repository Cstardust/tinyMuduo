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

//  创建wakeupFd,用来notify唤醒subReactor处理新来的Channel
int createEventfd()
{
    int evtfd = ::eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC);  
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
      callingPendingFunctors_(false),
      mtx_()
{
    LOG_DEBUG("EventLoop created %p in thread %d\n",this,threadId_);

    //  one loop per thread
    if(t_loopInThisThread != nullptr)
    {
        LOG_FATAL("Another EventLoop %p is existed in this thread %d\n",getEventOfCurrentThread(),threadId_);
    }
    else
    {
        t_loopInThisThread = this;
    }
    
    //  每个EventLoop都监听wakeupFd读事件 
    wakeupChannel_->enableReading();       
    wakeupChannel_->setReadCallback(std::bind(&EventLoop::handleWakeUpFd,this,std::placeholders::_1));
}



// loop -> epoll_wait -> handleEvent(IO线程监听到的event) -> doPendingFunctor()(其他worker线程加入的event cb)
//          epoll_wait 可能被eventfd(wakeupFd_)唤醒 为了 quit 或者执行其他worker线程加入的cb doPendingFunctor()
//  EventLoop核心
//  只有创建了EventLoop Object的thread(我们称之为eventloop所属的thread)，才有资格调用该object的loop方法。也即，才有资格作为main/subReactor的IOloop,监听处理 新连接(mainReactor) / 已连接socket发生的事件(subReactor)。
    //  通过assertInloopThread保证这点
void EventLoop::loop()
{
    assert(!looping_);
    //  保证是eventloop所属的thread使用该eventloop的loop方法
    assertInLoopThread();

    looping_ = true;
    quit_ = false;

    LOG_INFO("EventLoop %p belonged to thread %d start looping on thread %d",this,threadId_,CurrentThread::tid());

    while(!quit_)
    {
        activeChannels_.clear();

        //  监听两类fd
            //  一种是 listenfd / connfd
            //  一种是 **wakeupfd**
        pollReturnTime_ = poller_->poll(kPollerTimeMs,&activeChannels_);
        
        //  对于mainReactor的IO loop , 负责监听listenfd , handler : 接收新来的连接 并将新来的连接派发给subReactor(其他thread的eventloop)
        //  对于subReactor的IO loop  , 负责监听connfd   , handler : 处理 已连接socket上发生的事件.可以在subreactor的ioloop中处理 也可将任务打包提交给线程池处理
        for(Channel *channel:activeChannels_)
        {
            LOG_DEBUG("%d is being handled on EventLoop %p in thread %d\n",channel->fd(),this,CurrentThread::gettid());
            channel->handleEvent(pollReturnTime_);
        }

        //  执行添加给本eventLoop的回调操作(可能是本thread或其他thread)
        //  mainReactor IO 线程 accept fd=>channel  --分发给-->subloop
        doPendingFunctors(); 
    }

    looping_ = false;

    LOG_INFO("EventLoop %p belonged to thread %d stop looping on thread %d\n",this,threadId_,CurrentThread::tid());
}


//  退出事件循环
    //  在loop所属的线程(IO线程)中，loop.quit()。
        //  那么，只需要修改quit_ = true;。当loop的while下一次while(!quit)时，就会生效quit
    //  在非loop所属的线程(IO线程)中，loop.quit()。
        //  那么，只修改quit_ 的话，会有长达数秒的延迟。
        //  因为当前线程不是IO线程，需要切回IO线程，再执行到while的下一次while(!quit)，才会生效quit
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
}


//  当有数据写入eventfd时，epoll_wait会监听到这个读事件 于是eventloop就可以从epoll_wait的阻塞中离开
//  然后IO线程处理这个eventfd的读事件，调用之前注册的handleRead 将数据读出，然而并没有做什么其他的。
    //  虽然读出的数据没什么用，但是如果不处理这个eventfd的读事件的话 由于epoll是LT模式，就会不断触发这个wakeup读事件 造成busyloop
void EventLoop::handleWakeUpFd(const Timestamp& )
{
    LOG_DEBUG("handle wakeupFd\n");
    uint64_t one = 1;
    ssize_t n = read(wakeupFd_,&one,sizeof one);
    if(n!=sizeof one)
    {
        LOG_ERROR("EventLoop::handleRead() reads %ld bytes instead of 8\n",n);
    }
}



//  runInLoop -> IO cb()
//            -> Worker queueInLoop(cb)->wakeup->IO cb()  
//  让loop对象执行回调函数cb
    //  如果 调用loop.runInloop(cb)的线程 正好是loop对象所属的线程(IO线程)
        //  那么 直接在该loop上执行cb
    //  如果 调用loop.runInloop(cb)的线程 不是loop对象所属的线程(IO线程)
        //  那么 将cb存入loop所属的IO线程的回调队列。
        //  loop对象会待会在loop的IO线程调用这些回调cb
    //  cb：event handler
void EventLoop::runInLoop(const Functor& cb)
{
    LOG_DEBUG("cb runInLoop in loop %p on thread %d",this,CurrentThread::gettid());
    //  如果 当前正在运行的线程是创建EventLoop的线程(IO线程)
        //  那么，直接调用回调函数
    //  也即，如果用户在当前IO线程调用这个函数，回调会同步进行
    if(isInLoopThread())
    {
        LOG_DEBUG("直接调用cb on loop %p in thread %d\n",this,CurrentThread::gettid());
        cb();
    }
    else
    {
        queueInLoop(cb);
    }
}


//  把cb放入队列 (wakeupfd唤醒loop所在线程 执行cb)
void EventLoop::queueInLoop(const Functor& cb)
{
    LOG_DEBUG("queueInLoop");
    //  给loop添加新回调
    //  这个cb 会在loop的IO thread的epollwait之后 被调用
    {
        std::lock_guard<std::mutex> lock(mtx_);
        pendingFunctors_.emplace_back(cb);
    }

    //  只有当前queueInLoop()函数是被 loop所属的IO thread 执行cb中被调用，才无须wakeup()!
    //  callingPendingFactor = true 代表loop当前正在执行回调 没有阻塞在epoll_wait上
        //  这时新回调已经加入了loop的pendingFunctors，那么由于loop会执行完pendingFunctors中所有的cb
        //  所以 也就会执行新加入的cb
        //  当然无须再wakeup 那个 loop
    if(!isInLoopThread() || callingPendingFunctors_)          
    {
        //  在其他thread 通过loop对象wakeup方法 唤醒卡在epoll wait的loop的IOthread
        wakeup();                                          
    }
}



//  用来唤醒loop所在的(所属于的)线程(IO线程)。向wakeupFd_写一个数据。
//  wakeupChannel就发生读事件 当前loop线程就会被唤醒。
void EventLoop::wakeup()
{
    LOG_DEBUG("wakeup");
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
    callingPendingFunctors_ = true;      //  loop是否有需要执行的回调操作
    {
        std::lock_guard<std::mutex> lock(mtx_);
        functors.swap(pendingFunctors_); //  交换。functor获取所有需要执行的cb。pendingFunctors置空
    }
    LOG_DEBUG("functor size = %lu",functors.size());
    for(const Functor &functor : functors)
    {
        functor();                      //  在本loop thread中执行当前loop需要执行的回调操作
    }

    callingPendingFunctors_ = false;
}



void EventLoop::updateChannel(Channel *channel)
{
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    poller_->removeChannel(channel);
}

bool EventLoop::hasChannel(Channel* channel) const
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
        abortNotInLoopThread(); 
    }
}
void EventLoop::abortNotInLoopThread()
{
  LOG_FATAL("EventLoop::abortNotInLoopThread - EventLoop %p  was created in threadId_ = %d , current thread id = %d\n",this,threadId_,CurrentThread::tid());
}