#ifndef MY_MUDUO_ACCEPTOR
#define MY_MUDUO_ACCEPTOR

#include <functional>
#include "InetAddress.h"
#include "Socket.h"
#include "Channel.h"

class EventLoop;

class Acceptor
{
public:
    using NewConnectionCallback = std::function<void(int connfd, const InetAddress &cliet_addr,const InetAddress &localAddr)>;

public:
    Acceptor(EventLoop *loop, const InetAddress &server_addr, bool request_port);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback &cb) { newConnectionCallback_ = cb; }

    bool listening() const { return listening_; }
    void listen();
private:
    void handleConnection(Timestamp);
private:
    EventLoop *loop_; //  Acceptor使用的就是用户定义的baseloop,也就是mainloop
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;   
    bool listening_;
};

#endif