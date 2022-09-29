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

//  EventLoopThreadPool 经典！ one loop per thread

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
    TcpServer(EventLoop* loop,const InetAddress &server_addr,const string &name,Option op = kNoReusePort);
    ~TcpServer();

    //  开启server thread safe
    //  starts server if it's not listening
    //  it's harmless to call it multiple times
    void start();
    //  设置底层subloop个数
    void setThreadNum(int subThreadNum);

    void setThreadInitCallback(const ThreadInitCallback &server_addr = ThreadInitCallback());
    void setConnectionCallback(const ConnectionCallback& = ConnectionCallback());
    void setMessageCallback(const MessageCallback&  = MessageCallback());
    void setWriteCompleteCallback(const WriteCompleteCallback& = WriteCompleteCallback());
private:
    //  not thread safe , but in loop
    void newConnection(int sockfd,const InetAddress& cliet_addr);
    void removeConnection(const TcpConnection &conn);    
private:
    using ConnectionMap = std::unordered_map<std::string,TcpConnectionPtr>;

    //  baseloop(main loop) ; the acceptor loop
    //  用户定义的loop
    EventLoop *loop_;
    const std::string ipPort_; 
    const std::string name_;

    //  Acceptor Ptr    
        //  avoid revealing Accetor
        //  运行在baseloop 即 mainloop
        //  task：epoll_wait 监听新连接事件
    std::unique_ptr<Acceptor> acceptor_;

    //  Loop + Thread Pool
        //  one loop per thread!!!!!!
    std::shared_ptr<EventLoopThreadPool> threadPool_;
        
    //  设置callback
        //  连接回调
    ConnectionCallback connectionCallback_;
        //  接收消息回调
    MessageCallback    messageCallback_;
        //  消息发完之后的回调
    WriteCompleteCallback   WriteCompleteCallback_;
    ThreadInitCallback      threadInitCallback_;

    //  开启Server
    bool started_;
    //  always in loop
    int nextConnId_;  
    //  connPtrs
    ConnectionMap connections_;  
};


#endif