#ifndef MY_MUDUO_TCP_CONNECTION
#define MY_MUDUO_TCP_CONNECTION

#include <memory>
#include <cstring>
#include <atomic>
#include "InetAddress.h"
#include "Callback.h"
#include "Buffer.h"
#include "noncopyable.h"

class EventLoop;
class Socket;
class Channel;

//  acceptor : mainloop中的 监听listenfd
//  tcpconntcion : subloop中的 监听已连接connfd的读写事件

//  负责和一个connfd进行通信？
/*
 * TcpServer -> Acceptor -> 有一个新用户连接，通过accept拿到connfd
 * -> TcpConnection设置回调 -> Channel -> Poller -> Channel call back
 *
 */
class TcpConnection
    : private noncopyable , public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *loop,
                  const std::string &name,
                  int sockfd,
                  const InetAddress &localAddr,
                  const InetAddress &peerAddr);
    ~TcpConnection();

    EventLoop *getLoop() const { return loop_; }
    const std::string &name() const { return name_; }
    const InetAddress &localAddress() const { return localAddr_; }
    const InetAddress &peerAddress() const { return peerAddr_; }

    bool connected() const { return state_ == kConnected; }
    //  发送数据
    void send(const std::string& buf);
    //  关闭连接
    void shutdown();
    
    //  用户设置的回调函数
    void setConnectionCallback(const ConnectionCallback &cb) { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback &cb) { messageCallback_ = cb; }
    void setWriteCompleteCallback(const ConnectionCallback &cb) { writeCompleteCallback_ = cb; }
    void setHighWaterMarkCallback(const HighWaterMarkCallback &cb) { highWaterMarkCallback_ = cb; }
    void setCloseCallback(const CloseCallback &cb) { closeCallback_ = cb; }
    
    void connectionEstablished();
    void connectionDestroyed();
private:
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

    void sendInLoop(const void *message, size_t ken);
    void shutdownInLoop();
private:
    enum StateE
    {
        kDisconnected,
        kConnecting,
        kConnected,
        kDisconnecting
    };

    void setState(StateE s){state_ = s;}

    EventLoop *loop_; //  这里可不是baseloop 因为TcpConnection是subloop 。即 subthread的 即 subreactor的处理连接connfd的loop
    const std::string name_;
    std::atomic<int> state_;
    bool reading_;

    //  socket(fd and action on the fd) and channel(fd event handler)
    std::unique_ptr<Socket> socket_;   //  connfd ?
    std::unique_ptr<Channel> channel_; //  channel for connfd to poller?

    const InetAddress localAddr_;       //  server_addr
    const InetAddress peerAddr_;       //  client_addr        

    //  下面这些应当是都是user设定的回调
    //  user设定给TcpServer TcpServer设置给TcpConnection
    //  之后会在注册给channel的回调中被调用
    //  connectioncallback_ : user设定的回调
    //  在连接建立完成(accept完成，并将client fd的reading事件注册给指定loop的poller)后 
    //  会在 TcpConnection::connectionEstablished中调用 user设定的onConnection
    ConnectionCallback connectionCallback_;       //  连接建立和断开时的回调
    //  messageCallback_ : user设置的回调
    MessageCallback messageCallback_;             //  收到消息时的回调
    WriteCompleteCallback writeCompleteCallback_; //  消息发送完成之后的回调
    HighWaterMarkCallback highWaterMarkCallback_;
    CloseCallback closeCallback_;

    size_t highWaterMark_;

    Buffer inputBuffer_;    //  接收数据的缓冲区
    Buffer outputBuffer_;   //  发送数据的缓冲区
};

#endif