#include "Channel.h"
#include "Acceptor.h"
#include "Logger.h"
#include<unistd.h>
#include "CurrentThread.h"

static int createNonBlockingSocket()
{
    //  参数含义   //  tcp流       nonblocking     close socket_fd as soon as execve
    int listening_fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (listening_fd < 0)
    {
        LOG_INFO("failed to create server's listening socket\n");
    }
    return listening_fd;
}


Acceptor::Acceptor(EventLoop *loop, const InetAddress &server_addr, bool request_port)
    : loop_(loop),
      acceptSocket_(createNonBlockingSocket()),             //  create a socket listening fd
      acceptChannel_(loop_, acceptSocket_.fd()),            //  create a channel with main loop and listening fd
      newConnectionCallback_(NewConnectionCallback()),
      listening_(false)
{
    //  set listening fd
        //  reuse addr
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(true);
    acceptSocket_.setKeepAlive(false);  // may
    acceptSocket_.setTcpNoDelay(false); // may
        //  bind listening socketfd with server_addr(user set)
    acceptSocket_.bindAddress(server_addr);

    //  regisiter to epoll
        //  1. set call back for connect to listening fd
        //  将Acceptor内置的handleRead注册在poller上
        //  handleRead 先accept连接 后又会调用用户注册在Acceptor的回调函数
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleConnection,this,std::placeholders::_1));

}


void Acceptor::listen()
{
    listening_ = true;
    acceptSocket_.listen();             //  开启listen队列 之后就等待connect
    //  regisiter to epoll
        //  必须在listen 创建监听队列之后 再注册到epoll事件表上。
        //  2. 注册到epoll的事件表上
        //  使得acceptChannel 在 socket可读的时候调用 acceptor::handleRead()
        //  在handlRead里面 会accept 并调用 newConnectionCallback
    acceptChannel_.enableReading();    
}

//  当poller epoll wait 监听到连接事件时 会调用该回调
    //  1. handleConnection : accept连接 得到新connfd
    //  2. newConnectionCallback(TcpServer绑定的) : 本mainReactor选择一个subReactor 将
void Acceptor :: handleConnection(Timestamp)
{
    LOG_INFO("Acceptor::accept on thread %d",CurrentThread::gettid());
    InetAddress peerAddr;
    //  accept处理连接事件 返回fd  
        //  accept策略：一次accept一个 / 循环accept,直到没新连接 / 每次accept(N=10)个连接 
        //  后两种适合短连接
        //  muduo是为了长连接
        //  muduo p302
    int connfd = acceptSocket_.accept(&peerAddr);
    // 获取connfd socket绑定的本机的ip地址和port信息
    InetAddress localAddr(InetAddress::getSockName(connfd));
    //  perrAddr 是连接的client的socket的地址信息
    //  localAddr 是本地负责连接client的connfd socket的地址信息
    
    if(connfd >= 0)
    {
        if(newConnectionCallback_)
        {
            //  参数的传递问题
                //  值拷贝： 简单 不出错
                //  高效 ： 移动语义
                    //  这里可以用Socket 然后移动语义转移给newConnectionCallback ?  说是可以确保资源安全释放？？
                    //  那不会造成在移动的时候就调用了Socket的析构函数从而关闭了connfd吗？
            newConnectionCallback_(connfd,peerAddr,localAddr); //  轮询subloop 唤醒 分发当前新client的channel
        }
        else
        {
            LOG_ERROR("newConnectionCallback_ is nullptr");
            ::close(connfd);
        }
    }
    else
    {
        LOG_ERROR("failed to accept : %d\n",errno);
        if(errno == EMFILE)         //  可打开的文件描述符达到上限
        {
            LOG_ERROR("num of fds reached limit : %d\n",errno);
        }
    }
}


Acceptor::~Acceptor()
{
    acceptChannel_.disableAll();
    acceptChannel_.remove();
    //  stack acceptSocket 会在析构时自动close
}