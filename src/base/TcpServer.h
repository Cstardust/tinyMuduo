#ifndef MY_MUDUO_TCP_SERVER
#define MY_MUDUO_TCP_SERVER

#include <unordered_map>
#include <string>
#include <memory>
#include "TcpConnection.h"
#include "InetAddress.h"
#include "noncopyable.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "Callback.h"
#include "EventLoopThreadPool.h"

//  user如何使用TcpServer?
//  设置好callback
//  调用start()即可

//  TcpServer --> EventLoop --> Channel
//                          --> Poller
class TcpServer : noncopyable
{
public:
    enum Option
    {
        kNoReusePort,
        kReusePort,
    };

public:
    TcpServer(EventLoop *loop, const InetAddress &server_addr, const string &name, Option op = kNoReusePort);
    ~TcpServer();

    //  starts server if it's not listening , it's harmless to call it multiple times
    void start();
    //  设置底层subloop个数
    void setThreadNum(int subThreadNum);

    //  这些都是 交由用户 自由设置 的 函数。可以在连接建立/收到消息等时候调用
    //  user自己编写这几个函数 然后设置给tcpServer。
    //  TcpServer将这些callback 设置给每个建立的tcpConnectionPtr
    //  Channel会取用TcpConnectionPtr的这几个函数
    void setThreadInitCallback(const ThreadInitCallback &cb = ThreadInitCallback()) { threadInitCallback_ = cb; }
    void setConnectionCallback(const ConnectionCallback &cb = ConnectionCallback()) { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback &cb = MessageCallback()) { messageCallback_ = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback &cb = WriteCompleteCallback()) { writeCompleteCallback_ = cb; }

private:
    //  not thread safe , but in loop
    void newConnection(int sockfd, const InetAddress &peerAddr, const InetAddress &localAddr);
    void removeConnection(const TcpConnectionPtr &conn);
    void removeConnectionInLoop(const TcpConnectionPtr &);

private:
    using ConnectionMap = std::unordered_map<std::string, TcpConnectionPtr>;

    //  baseloop(main loop) ; the acceptor loop
    //  用户定义的loop 是在TcpServer类外构造的对象 故其生命周期不必由TcpServer管理
    EventLoop *loop_;
    const std::string ipPort_;
    const std::string name_;

    //  Acceptor Ptr
    //  avoid revealing Accetor
    //  运行在baseloop 即 mainloop
    //  task：epoll_wait 监听新连接事件
    std::unique_ptr<Acceptor> acceptor_;

    //  Loop + Thread Pool : subReactors pool
    std::shared_ptr<EventLoopThreadPool> threadPool_;

    //  设置callback
    //  连接回调
    ConnectionCallback connectionCallback_;
    //  接收消息回调
    MessageCallback messageCallback_;
    //  消息发完之后的回调
    WriteCompleteCallback writeCompleteCallback_;
    ThreadInitCallback threadInitCallback_;

    //  开启Server
    bool started_;
    //  always in loop
    int nextConnId_;
    //  connPtrs
    ConnectionMap connections_;
};

#endif