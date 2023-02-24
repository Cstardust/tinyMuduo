#include "TcpServer.h"
#include "Logger.h"
#include <functional>

static EventLoop *checkLoopNotNull(EventLoop *loop)
{
    if (loop == nullptr)
    {
        LOG_FATAL("TcpConnection Loop is null!\n");
    }
    return loop;
}

TcpServer::TcpServer(EventLoop *loop,
                     const InetAddress &server_addr,
                     const string &name,
                     Option op /*= kNoReusePort*/) 
      : loop_(checkLoopNotNull(loop)),
        ipPort_(server_addr.toIpPort()),
        name_(name),
        acceptor_(new Acceptor(loop, server_addr, false)),
        threadPool_(new EventLoopThreadPool(loop, name)),
        connectionCallback_(),
        messageCallback_(),
        writeCompleteCallback_(),
        started_(false),
        nextConnId_(1)
{
    //  acceptor 绑定了 TcpServer的newConnection
    //  TcpServer::newConnection 绑定给acceptor
    //  acceptor接收新连接之后 将该连接分发给ioloop
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this,
                                                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

TcpServer::~TcpServer()
{
    LOG_INFO("~TcpServer()");
    for (auto &item : connections_)
    {
        //  这个局部的shared_ptr智能指针对象，出右括号，可以自动释放原本item.second指向的tcpconnection
        TcpConnectionPtr conn(item.second);
        //  原本的指向tcpconnection强智能指针reset 不再指向tcpconnection
        item.second.reset();
        //  销毁连接
        conn->getLoop()->runInLoop(
            std::bind(&TcpConnection::connectionDestroyed, conn));
    }
}

void TcpServer::setThreadNum(int subLoopNum)
{
    threadPool_->setThreadNum(subLoopNum);
}

//  在mainthread(basethread)(就是用户使用tcpserver的这个thread)中创建user之前设定好个数的subloop 并开启subloop的loop() 
//  mainthread执行一下listening socket的listen
//  并没有开启mainthread上的eventloop的loop
//  mainloop的loop是由user开启的.
    //  感觉也可以改成由start开启
void TcpServer::start()
{
    if (started_++ == 0) // 1. atomic 2. ++防止一个TcpServer被start多次
    {
        //  启动底层线程池 : 创建subloop 并调用subloop.loop()  
        threadPool_->start(threadInitCallback_);
        //  baseloop  listen
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
    }
}

// 有一个新的客户端的连接，eventloop 会调用acceptor的handleConnection handleConnection会执行这个回调操作
void TcpServer::newConnection(int connfd, const InetAddress &peerAddr, const InetAddress &localAddr)
{
    LOG_DEBUG("connfd = %d",connfd);
    // 轮询算法，选择一个subLoop，来管理channel
        //  user没有设置subloop的话，那么ioloop 即 mainloop
    // one loop per thread 思想实现多线程TcpServer 关键步骤：
    //  在新建TcpConnection时 ，从event loop pool中挑选一个loop来给TcpConnection用
    //  即 TcpServer自己的loop只用来接受新连接，而新连接会用其他EventLoop来执行IO
    EventLoop *ioLoop = threadPool_->getNextLoop();

    //  connectionName
    char buf[64] = {0};
    snprintf(buf, sizeof buf, "-%s#%d", ipPort_.c_str(), nextConnId_++);     //  易知这个listenfd的ipPort 和 connfd (peerAddr)的是一样的
    std::string connName = name_ + buf;
    LOG_DEBUG("TcpServer::newConnection [%s] - new connection [%s] from %s \n",
             name_.c_str(), connName.c_str(), peerAddr.toIpPort().c_str());
    
    //  将连接派发给选择ioloop
        //  根据连接成功的sockfd，创建TcpConnection连接对象
    TcpConnectionPtr conn(new TcpConnection(
        ioLoop,
        connName,
        connfd,
        localAddr,
        peerAddr));

    // 下面的callback都是user设置给TcpServer =>TcpConnection=>Channel=>Poller=>notify channel调用回调
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    // 设置了如何关闭连接的回调   conn->shutDown()
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));

    // LOG_DEBUG("shared_ptr cnt %ld",conn.use_count());
    
    //  记录在TcpServer中 connName - Tcpcpnnection conn
        //  conn引用计数 ++ 
    connections_[connName] = conn;
    
    // LOG_DEBUG("shared_ptr cnt %ld",conn.use_count());

    //  如果ioloop是mainloop 那么就是在eventloop所属thread上调用runInloop方法 直接cb即可
    //  如果ioloop是subloop 那么唤醒subthread的subloop执行connectionEstablished
    ioLoop->runInLoop(std::bind(&TcpConnection::connectionEstablished, conn));
}

//  removeConnection -> removeConnectionInLoop
void TcpServer::removeConnection(const TcpConnectionPtr &conn)
{
    //  TcpServer的loop_必定是个mainloop阿 
    loop_->runInLoop(
        std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

//  handleRead n = 0 -> handleClose - >removeConnection -> removeConnectionInLoop -> TcpConnection::connectionDestroyed
void TcpServer::removeConnectionInLoop(const TcpConnectionPtr &conn)
{
    // LOG_DEBUG("shared_ptr cnt %ld",conn.use_count());
    LOG_INFO("TcpServer::removeConnectionInLoop [%s] - connection %s",
             name_.c_str(), conn->name().c_str());
    //  tcpserver 从记录中删除该连接
    connections_.erase(conn->name());       //  tcpconnection use count -- 

    // LOG_DEBUG("after erase shared_ptr cnt %ld",conn.use_count());
    //  将tcpconnection的生命周期 延迟到connectionDestroyed时刻
        //  参数绑定传智能指针！！而非裸指针！！不然会导致其提前析构！！
        //  **智能指针 可以作为成员函数隐含的this**
    //  获取conn所属的subloop
    EventLoop *ioLoop = conn->getLoop();
    // ioLoop->queueInLoop(
    //     std::bind(&TcpConnection::connectionDestroyed,conn));
    ioLoop->runInLoop(
        std::bind(&TcpConnection::connectionDestroyed,conn));
    // LOG_DEBUG("after removeConnectionInLoop");
}