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
    //  owner is TcpConnection
    if(tied_)
    {
        //  lock
        //  延长TcpConnection生命周期，保证不在handleEvent时析构
        std::shared_ptr<void> guard = tie_.lock();
        if(guard)
        {
            handlerEventWithGuard(receiveTime);
        }
    }
    //  owner is Acceptor
    else
    {
        handlerEventWithGuard(receiveTime);
    }
}


//  根据poller通知的channel发生的具体事件，由channel负责调用具体的回调操作
void Channel::handlerEventWithGuard(const Timestamp& receiveTime)
{
    //  对端关闭?
    if((!(revents_ & EPOLLIN)) && (revents_ & EPOLLHUP)) 
    {
        LOG_INFO("happened 1\n");
        if(closeCallback_!=nullptr)
        {
            closeCallback_();
        }
    }
    //  server error?
    if(revents_ & EPOLLERR)
    {
        LOG_INFO("happened 2\n");
        if(errorCallback_)
        {
            errorCallback_();
        }
    }
    //  普通的读事件
    if(revents_ & (EPOLLIN | EPOLLPRI))
    {
        LOG_INFO("happened 3\n");
        if(readCallback_)
        {
            readCallback_(receiveTime);
        }
    }
    //  写事件：从buffer中向tcp socket的写缓冲区中写数据
    if(revents_ & EPOLLOUT)
    {
        LOG_INFO("happened 4\n");
        if(writeCallback_)
        {
            writeCallback_();
        }
    }
}

