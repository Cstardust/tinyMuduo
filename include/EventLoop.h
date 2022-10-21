#ifndef MY_MUDUO_EVENT_LOOP
#define MY_MUDUO_EVENT_LOOP


//  EventLoop : the core of reactor

#include<atomic>
#include<memory>
#include<vector>
#include<mutex>
#include<functional>
#include"CurrentThread.h"
#include"Timestamp.h"


class Poller;
class Channel;


class EventLoop: noncopyable
{
public:
    using Functor = std::function<void()>;
    
public:
    EventLoop();
    ~EventLoop();

    //  开启事件循环
    void loop();
    //  退出事件循环
    void quit();

    Timestamp pollReturnTime() const {return pollReturnTime_;}

    //  在当前loop中执行cb
    void runInLoop(const Functor& cb);         
    //  把cb放入队列 等待loop执行
    void queueInLoop(const Functor& cb);      

    //  讲loop从wait中唤醒
    void wakeup();      

    //  EventLoop -> Poller
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel*) const;
    //  判断 创建该EventLoop的线程 是不是当前正在CPU上运行的线程。
    bool isInLoopThread() const{return threadId_ == CurrentThread::tid();}

    static EventLoop *getEventOfCurrentThread();
    
    void assertInLoopThread();

    //  定时器相关操作....
private:
    //  解除wakeup事件
    void handleWakeUpFd(const Timestamp&);
    //  执行回调
    void doPendingFunctors();
    void abortNotInLoopThread();
private:
    using ChannelList = std::vector<Channel*>; 
    //  控制相关
    std::atomic<bool> looping_;         //  是否开启循环 CAS原子操作  
    std::atomic<bool> quit_;            //  循环是否退出(比如当client掉的时候 loop就要退出？？) 
    
    //  标识创建了EventLoop线程的id
    const pid_t threadId_;              //  记录创建loop的线程的tid
   
    //  Poller相关 
    std::unique_ptr<Poller> poller_;
    Timestamp pollReturnTime_;          //  poller返回发生事件的channels的时间点

    //  wakeupFd!!!!!!!!!!!! muduo库最重要的知识点!!
    //  mainReactor 如何唤醒subReactor?? **通过 wakeupFd 统一事件源！！！**
    // 每个subReactor 监听了 一个 wakeupChannel
    // mainReactor 通过 给wakeupChannel发送消息 让subReactor感知到wakeupFd上有读事件发生。
    // subReactor 就能拿到mainReactor给的新连接的fd
    int wakeupFd_;                              //  主要作用：当mainLoop获取一个新用户的channel，通过轮询算法选择一个subLoop 通过wakeupFd唤醒subLoop处理channel
    std::unique_ptr<Channel> wakeupChannel_;    //  wakeupChannel_ 封装wakeupFd以及其事件

    ChannelList activeChannels_;        //  EventLoop 管理的channels
    Channel*   currentActiveChannel_;   //  for assert
    
    std::atomic<bool> callingPendingFunctors_;   //  loop_ is doPendingFunctor
    std::vector<Functor> pendingFunctors_;      //  存储loop需要执行的所有回调操作  干嘛的？
    std::mutex mtx_;                            //  互斥锁 用来保护上面vector的线程安全操作
};


#endif


/*
eventfd 效率很高！！
eventfd()  creates an "eventfd object" that can be used as an event wait/notify mechanism by user-space applications, and by the kernel to notify user-space applications
       of events.  The object contains an unsigned 64-bit integer (uint64_t) counter that is maintained by the kernel.  This counter is initialized with the value specified  in
       the argument initval.

*/