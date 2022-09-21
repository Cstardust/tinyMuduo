#ifndef MY_MUDUO_POLLER_EPOLL_H
#define MY_MUDUO_POLLER_EPOLL_H

#include"Poller.h"
#include<vector>
#include<sys/poll.h>

/*
 * 
 * epoll_create(epollfd)     ---- EpollPoller()
 * epoll_ctl(mod add del)    ---- mod add : update() del : remove
 * epoll_wait                ---- poll()
 * 析构时close epollfd  
 * 
 */

class EpollPoller : public Poller
{
public:
    EpollPoller(EventLoop* loop);
    //  override 代表 子类重写父类函数 也即，父类一定会有这个同名的虚函数
    ~EpollPoller() override;                                            
    //  epoll_wait
        //  获取当前活动的IO事件，并填充给传入的ChannelList
    Timestamp poll(int timeoutMs,ChannelList *activeChannels) override;      
    //  epoll_ctl 增加fd 将fd以及其感兴趣的事件注册在epoll事件表上
    void updateChannel(Channel *) override;
    //  epoll_ctl 从epoll事件表上删除fd  
    void removeChannel(Channel *) override;
private:
    void update(int operation,Channel* channel);
    void fillActiveChannels(int numEvents,ChannelList* activeChannels) const;
private:
    using EventList = std::vector<epoll_event>;
    static const int kInitEventListSize = 16;           //  epoll_event events[SIZE]大小 初始默认监听多少个事件？
    //  epoll_event 数组 内核存储发生事件的集合 动态扩容
    EventList events_;
    //  epfd 构造函数中epoll_create
    int epollfd_;               
};



#endif