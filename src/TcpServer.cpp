#include "TcpServer.h"

TcpServer::TcpServer(EventLoop *loop,
                     const InetAddress &server_addr,
                     const string &name,
                     Option op/*= kNoReusePort*/): 
                        loop_(loop),
                        ipPort_(server_addr.toIpPort()),
                        name_(name),
                        acceptor_(new Acceptor(loop, server_addr, false)),
                        threadPool_(new EventLoopThreadPool(loop, name)),
                        connectionCallback_(),
                        messageCallback_(),
                        WriteCompleteCallback_(),
                        started_(false),
                        nextConnId_(1)
{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this,
                                                  std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer(){}




void TcpServer::newConnection(int sockfd,const InetAddress& cliet_addr)
{

}


void TcpServer::setThreadNum(int subLoopNum)
{
    threadPool_->setThreadNum(subLoopNum);
}

//  开启server监听 loop.loop()
void TcpServer::start()
{
    if(started_++ == 0)     // 1. atomic 2. ++防止一个TcpServer被start多次
    {
        //  启动底层线程池 subloop全部启动
        threadPool_->start(threadInitCallback_);    
        //  main loop 开启
        loop_->runInLoop(std::bind(&Acceptor::listen,acceptor_.get()));
    }
}

void TcpServer::newConnection(int sockfd,const InetAddress & cliet_addt)
{
    
}

