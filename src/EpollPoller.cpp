#include"Logger.h"
#include"EpollPoller.h"
#include<fcntl.h>
#include<unistd.h>
#include<cassert>
#include<cstring>

//  Channel index_
// Channel 未被添加到 Poller中      不在channel_map中 && 没注册在epoll上
const int kNew = -1;            
// Channel 已被添加到 Poller中      在channel_map中 && 注册在epoll上
const int kAdded = 1;
// Channel 从 Poller 中 删除        在channel_map中 && 没注册在epoll上
const int kDeleted = 2;
//  kDeleted 有什么用？？？？？

EpollPoller::EpollPoller(EventLoop *loop)
    :Poller(loop),                   
    events_(kInitEventListSize),    //  epoll_events[size]
    epollfd_(::epoll_create1(EPOLL_CLOEXEC))    //  child will close epfd on execve
{
    if(epollfd_ < 0)
    {
        LOG_FATAL("epoll_create error : %d\n",errno);   //  crash
    }
}

// 创建socket负责的吧这个应该是
// int setnonblocking(int fd)
// {
//     int old_option = fcntl(fd,F_GETFL);
//     int new_option = old_option | O_NONBLOCK;
//     fcntl(fd,F_SETFL,new_option);
//     return old_option;
// }

EpollPoller::~EpollPoller()
{
    ::close(epollfd_);
}

// channel update -> EventLoop updateChannel -> Poller updateChannel -> EpollPoller
void EpollPoller::updateChannel(Channel *channel)
{
    int index = channel->index();
    LOG_INFO("fd = %d , events = %d , index = %d\n",channel->fd(),channel->events(),index);
    
    if(index == kNew || index == kDeleted)
    {
        //  kNew 
        //  insert into the fd-channel map
        if(index == kNew)
        {
            int fd = channel->fd();
            channels_.insert({fd,channel});
        }
        //  index of channel to be kAdded beacause of it will be added to the epoll
        channel->set_index(kAdded);
        //  将Channel封装的fd以及fd的相应事件 ADD注册 在EPOLL上
        update(EPOLL_CTL_ADD,channel);  
    }
    else if(index == kAdded)
    {
        int fd = channel->fd();
        assert(channel == channels_[fd]);

        //  fd 没有事件要监听 --> 从epoll中移除fd 然后 改变记录channel的状态 added->deleted
            //  added -> delete 只是从epoll中拿下，并没有改变channelMap. fd - channel仍旧记录在map中
            //  为什么不拿下？
        if(channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL,channel);
            channel->set_index(kDeleted);        
        }
        //  fd上仍有要监听的事件 只不过需要发生变化 需要更改
        else
        {
            update(EPOLL_CTL_MOD,channel);
        }
    }
    else
    {
        LOG_ERROR("unexpected index = %d",index);
    }
}

//  epoll_ctl
    //  operation    EPOLL_CTL_ADD , EPOLL_CTL_MOD , EPOLL_CTL_DEL
    //  channel      fd event and handler
            //  event : EPOLLIN / EPOLLOUT / EPOLLPRI
void EpollPoller::update(int operation,Channel* channel)
{
    epoll_event event;
    memset(&event,0,sizeof event);
    event.data.ptr = channel;           //  封装了 fd events and handler
    event.events = channel->events();
    int fd = channel->fd();
    if(epoll_ctl(epollfd_,operation,fd,&event) < 0)
    {
        if(operation == EPOLL_CTL_DEL)
        {
            LOG_ERROR("epoll_ctl del error %d\n",errno);
            return ;
        }
        else
        {
            LOG_FATAL("epoll_ctl mod/add error %d\n",errno);
            exit(-1);
        }
    }
}

void EpollPoller::removeChannel(Channel *channel)
{
    int fd = channel->fd();
    int index = channel->index();
    LOG_INFO("fd = %d , events = %d , index = %d\n",fd,channel->events(),index);

    assert(channel == channels_[fd]);
    update(EPOLL_CTL_DEL,channel);
}

//  EventLoop调用Poller的fillActiveChannels 获取Poller监听到的发生事件的Channel
void EpollPoller::fillActiveChannels(int numEvents,ChannelList* activeChannels) const
{
    for(int i=0;i<numEvents;++i)
    {
        //  获取监听到的channel
        Channel * channel = static_cast<Channel*>(events_[i].data.ptr) ;
        //  获取fd上实际发生的事件
        int event_happened = events_[i].events;        
        channel->set_revents(event_happened);  
        activeChannels->emplace_back(channel);
        //  EventLoop拿到了它的Poller负责监测的所有Channel中 发生事件的Channel
    }
}

Timestamp EpollPoller::poll(int timeoutMs,ChannelList *activeChannels) 
{
    //  实际上应该用LOG_DEBUG
    LOG_INFO("fd total count : %lu\n",channels_.size());
    //	int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
    //  epoll_wait  
        //  epoll_event[] 传入传出参数 用来存内核得到事件的集合，
        //  maxevents     epoll总共需要监听多少个events
    int numEvents = epoll_wait(epollfd_ , events_.begin().base() , static_cast<int>(events_.size()) , timeoutMs);
    
    //  保存errno 防止被其他错误修改
    int saveErrno = errno;
    
    Timestamp now(Timestamp::now());

    if(numEvents > 0)
    {
        LOG_INFO("%d events happneded\n",numEvents);
        fillActiveChannels(numEvents,activeChannels);
        if(numEvents == events_.size())
        {
            events_.resize(numEvents * 2);
        } 
    }
    else if(numEvents == 0)
    {
        LOG_INFO("%s timeout!\n","epoll_wait");
    }
    else 
    {
        //  如果是外部中断 无所谓 下次接着调用epoll_wait即可
        //  如果不是外部中断 那么就是error！
        if(saveErrno != EINTR)
        {
            errno = saveErrno;
            LOG_ERROR("epoll_wait error : %d\n",errno);
        }
    }
    return now;
}

