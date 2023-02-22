#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"

Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop),
      fd_(fd),
      events_(0),
      revents_(0),
      index_(-1),
      tied_(false)
{    
}

Channel::~Channel()
{}


void Channel::tie(const std::shared_ptr<void>& ptr)
{
    tie_ = ptr;
    tied_ = true;
}

//  Channel enable/disable -> update -> EventLoop -> Poller -> epoll_ctl
//  参数传this 是为了让poller建立起fd-Channel*的关系
//  毕竟根据channel*就可以获得fd和handler。将channel*上报给eventloop，eventloop可以知道如何处理事件
void Channel::update()
{
    loop_->updateChannel(this);
    //  Eventloop -> Poller -> epoll_ctl 更新fd注册在事件表上的event
}

//  EventLoop -> ChannelList
//  在channel所属的EventLoop中 把当前的channel删除掉
void Channel::remove()
{
    loop_->removeChannel(this);
}

//  Channel的核心
void Channel::handleEvent(const Timestamp& receiveTime)
{
    //  绑定了 && 没析构 handleEvent
    if(tied_)
    {
        std::shared_ptr<void> guard = tie_.lock();
        if(guard)
        {
            handlerEventWithGuard(receiveTime);
        }
    }
    //  没绑定 handleEvent
    else
    {
        handlerEventWithGuard(receiveTime);
    }
}

//  根据poller通知的channel发生的具体事件，由channel负责调用具体的回调操作
void Channel::handlerEventWithGuard(const Timestamp& receiveTime)
{
    //  对端关闭
    //    EPOLLHUP
    //       Hang  up  happened  on  the  associated  file descriptor.  epoll_wait(2) will always wait for this event; it is not necessary to set it in
    //       events.
    //       Note that when reading from a channel such as a pipe or a stream socket, this event merely indicates that the peer closed its end  of  the
    //       channel.  Subsequent reads from the channel will return 0 (end of file) only after all outstanding data in the channel has been consumed.
    if(!(revents_ & EPOLLIN) && (revents_ & EPOLLHUP)) 
    {
        LOG_DEBUG("happened 1\n");
        if(closeCallback_!=nullptr)
        {
            closeCallback_();
        }
    }
    //    EPOLLERR
    //           Error condition happened on the associated file descriptor.  This event is also reported for the write end of a pipe when the read end has
    //           been closed.  epoll_wait(2) will always report for this event; it is not necessary to set it in events.
    if(revents_ & EPOLLERR)
    {
        LOG_DEBUG("happened 2\n");
        if(errorCallback_)
        {
            errorCallback_();
        }
    }
    //  读事件
    if(revents_ & (EPOLLIN | EPOLLPRI))
    {
        LOG_DEBUG("happened 3\n");
        if(readCallback_)
        {
            readCallback_(receiveTime);
        }
    }
    //  写事件
    if(revents_ & EPOLLOUT)
    {
        LOG_DEBUG("happened 4\n");
        if(writeCallback_)
        {
            writeCallback_();
        }
    }
}

