#include"Poller.h"
#include"EpollPoller.h"
#include<stdlib.h>

// 为什么不把newDefaultPoller 写在Poller.cpp中？
// 如果我们要放在Poller.cpp 中 那么由于这个函数需要返回一个具体的Epoll或者Poll对象 那么势必会include"EpollPoller.h" 以及 “Poller.h" 这样不合理
// 因为Poller是基类 良好的风格应该是 派生类引用基类 而非基类引用派生类
// （因为应当是 抽象(稳定)不应该依赖于实现细节(变化) ，实现细节应该依赖于抽象(稳定)。）
// 通过将newDefaultPoller 放入公共文件 解决了基类和子类之间的强耦合
//  这样 当具体的子类如POLL.h 或者 EPOLL.h 改变时 
//  父类Poller.cc 不需要改变 ，也即不需要重新编译
//  需要重新编译的只有newDefaultPoller.cc

Poller *Poller::*newDefaultPoller(EventLoop *loop)
{
    if(::getenv("MY_MUDDUO_USE_POLL"))
    {
        return nullptr;     //  构造POLL对象
    }
    else
    {
        return nullptr;     //  构造EPOLL对象
    }
}

