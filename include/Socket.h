#ifndef MY_MUDUO_SOCKET
#define MY_MUDUO_SOCKET

#include "noncopyable.h"

class InetAddress;

//  Socket
    //  socketfd 的 RAII handler
        //  在构造函数中 创建fd资源 ; 在析构函数中 释放打卡的fd
//  acceptor 使用该 Socket
//  Socket 记录的资源仅仅是一个打开的Socket Fd 
class Socket : noncopyable
{
public:
    explicit Socket(int sockfd);
    ~Socket();
    //  管理socket fd进行的操作
    //  一开始的socket fd是由外界传入的。
    //  外界创建socket 并将fd传入给Socket对象进行记录 管理
    //  之后要对这个打开的socket进行操作就对这个Socket对象操作即可。
    //  先bind 再listen 然后epoll_wait 最后accept
    void bindAddress(const InetAddress &localaddr);
    void listen();
    int accept(InetAddress *peeraddr);

    int fd() const {return fd_;}
    //  对传入的打开的socket fd属性进行设定
    void shutdownWrite();
    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);
private:
    const int fd_;
};

#endif