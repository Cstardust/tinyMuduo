#ifndef MY_MUDUO_POLLER_EPOLL_H
#define MY_MUDUO_POLLER_EPOLL_H

#include"Poller.h"
#include<vector>
#include<sys/poll.h>



//  RAII of epoll fd
    //  create epoll fd in ctor
    //  close  epoll fd in dtor
    //  epoll create , epoll wait , epoll_ctl
class EpollPoller : public Poller
{
public:
    //  epoll_create
    EpollPoller(EventLoop* loop);
    ~EpollPoller() override;                                            
    //  epoll_wait , 将发生的事件通过ChannelList传递给loop
    Timestamp poll(int timeoutMs,ChannelList *activeChannels) override;      
    //  epoll_ctl  , 修改 或者 将fd以及其感兴趣的事件注册在epoll事件表上
    void updateChannel(Channel *) override;
    //  epoll_ctl  , 从epoll事件表上删除fd  
    void removeChannel(Channel *) override;
private:
    void update(int operation,Channel* channel);
    void fillActiveChannels(int numEvents,ChannelList* activeChannels) const;
private:
    using EventList = std::vector<epoll_event>;
    //  epoll_event events[SIZE]大小 默认监听多少event
    static const int kInitEventListSize = 16;           
    //  epoll_event 数组 内核存储发生事件的集合 动态扩容
    EventList events_;
    //  epfd 构造函数中epoll_create
    int epollfd_;               
};



#endif