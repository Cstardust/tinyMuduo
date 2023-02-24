#include<arpa/inet.h>
#include<sys/types.h>
#include<cstring>
#include<sys/socket.h>
#include <netinet/tcp.h>
#include<unistd.h>
#include "Socket.h"
#include"InetAddress.h"
#include "Logger.h"

Socket::Socket(int sockfd)
    : fd_(sockfd)
{
}


void Socket::bindAddress(const InetAddress &server_addr)
{
    const sockaddr_in *addr = server_addr.getSockAddr();
    int ret = ::bind(fd_,(sockaddr*)addr,sizeof (*addr));
    if(ret == -1)
    {
        LOG_FATAL("failed to bind addr with socket!\n");
    }
}


void Socket::listen()
{
    int ret = ::listen(fd_,1024);
    if(ret == -1)
    {
        LOG_FATAL("failed to create listening queue for fd %d!\n",fd_);
    }
}



//  peeraddr for connected address
//  return connfd
    //  cliet_addr是一个传入参数 在accept中构造好之后供外部使用 使得外部知道连接方的地址信息
int Socket::accept(InetAddress *clietaddr)
{
    sockaddr_in cliet_addr;
    socklen_t cliet_len = sizeof(sockaddr_in);
    memset(&cliet_addr,0,sizeof cliet_addr);
    //  设置建立的sockfd nonblocking + child process execve之后close
    int connfd = ::accept4(fd_,(sockaddr*)&cliet_addr,&cliet_len,SOCK_CLOEXEC | SOCK_NONBLOCK);
    if(connfd == -1)
    {
        LOG_ERROR("failed to accept the connection!\n");
    }
    clietaddr->setSockAddr(cliet_addr);
    return connfd;
}

//  禁用Nagle 算法 避免连续发包出现延迟。
void Socket::setTcpNoDelay(bool on)
{
    int opt = on ? 1 : 0;
    ::setsockopt(fd_,IPPROTO_TCP,TCP_NODELAY,&opt,sizeof opt);
}

//  复用
void Socket::setReuseAddr(bool on)
{
    int opt = on ? 1 : 0;
    ::setsockopt(fd_,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof opt);
}
//  复用
void Socket::setReusePort(bool on)
{
    int opt = on ? 1 : 0;
    ::setsockopt(fd_,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof opt);
}


// 定期探测tcp连接是否还存在
void Socket::setKeepAlive(bool on)
{
    int opt = on ? 1 : 0;
    ::setsockopt(fd_,SOL_SOCKET,SO_KEEPALIVE,&opt,sizeof opt);
}

Socket::~Socket()
{
    ::close(fd_);
}

void Socket::shutdownWrite()
{
    if (::shutdown(fd_, SHUT_WR) < 0)
    {
        LOG_ERROR("shutdownWrite error");
    }
}

/* 
Shut down all or part of the connection open on socket FD.
   HOW determines what to shut down:
     SHUT_RD   = No more receptions;
     SHUT_WR   = No more transmissions;
     SHUT_RDWR = No more receptions or transmissions.
Returns 0 on success, -1 for errors.  
*/
