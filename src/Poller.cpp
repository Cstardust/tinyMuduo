#include"Poller.h"

Poller::Poller(EventLoop* loop)
    :ownerLoop_(loop)
    {}


//  检测本Poller是否负责监测Channel(fd)的信息
bool Poller::hasChannel(Channel *channel) const
{
    ChannelMap::const_iterator citer = channels_.find(channel->fd());
    return citer!=channels_.begin();
}





