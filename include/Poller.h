#ifndef POLLER_H
#define POLLER_H

#include"noncopyable.h"
#include"EventLoop.h"
#include"Channel.h"
#include<unordered_map>
#include<vector>

//  Poller doesn't own Channel objects
//  封装了 epoll / poll / select
    //  epoll_create  epoll_ctl  epoll_wait
//  core function : poll
class Poller:noncopyable
{
public:
    using ChannelList = std::vector<Channel*>;
public:
    Poller(EventLoop*);
    //  注意析构函数virtual
    virtual ~Poller() = default;                                   
    
    //  给所有IO复用子类保留的统一接口
        //  (epoll_wait) polls the io events
        //  must be called in the loop thread
    virtual Timestamp poll(int timeoutMs,ChannelList *activeChannels) = 0;  //  epoll_wait 这应该就是封装的epoll_wait 传入timeout时间以及感兴趣的fd return发生的事件数量
        //  changes the instereted io events
        //  must be called ub the loop thread
        //  epoll_ctl add mod 更新Channel的fd所注册的事件. 必须在loop thread中调用
        //  目的：更新维护sockfd-channel数组 以及 epoll_ctl
    virtual void updateChannel(Channel *) = 0;                     
        //  epoll_ctl del remove the channel, when it destructs . 必须在loop thread中调用
    virtual void removeChannel(Channel *) = 0;                      

    //  判断该poller是否监听了该channel
    bool hasChannel(Channel *) const;

    //  EventLoop 可以通过该接口获取默认的Poller object
        //  产生Poller对象. 由于依赖子类，单独放入一个.cc文件
    static Poller *newDefaultPoller(EventLoop *loop);  

    //  判断当前Poller对象 是否属于 当前运行的线程
    void assertInLoopThread(); 
protected:
    using ChannelMap = std::unordered_map<int,Channel*>;//  sockfd -- Channel
    ChannelMap channels_;
private:
    //  该Poller所属的EventLoop
    //  protected 注意这是个基类 还有子类呢
    EventLoop *ownerLoop_;   
};


#endif