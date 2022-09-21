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


//  绑定什么？绑定poller吗？也不是啊
void Channel::tie(const std::shared_ptr<void>& ptr)
{
    tie_ = ptr;
    tied_ = true;
}

//  EventLoop -> Poller->epoll_ctl
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
    //  对端关闭?
    if(!(revents_ & EPOLLIN) && (revents_ & EPOLLHUP)) //  什么意思？？EPOLLHUP???
    {
        LOG_INFO("happened1\n");
        if(closeCallback_!=nullptr)
        {
            closeCallback_();
        }
    }
    //  server error?
    if(revents_ & EPOLLERR)
    {
        LOG_INFO("happened2\n");
        if(errorCallback_)
        {
            errorCallback_();
        }
    }
    //  普通的读事件
    if(revents_ & (EPOLLIN | EPOLLPRI))
    {
        LOG_INFO("happened3\n");
        if(readCallback_)
        {
            readCallback_(receiveTime);
        }
    }
}

