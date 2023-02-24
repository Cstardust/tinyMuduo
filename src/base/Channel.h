#ifndef MY_MUDUO_CHANNEL_H
#define MY_MUDUO_CHANNEL_H

/// A selectable I/O channel.
///
/// This class doesn't own the file descriptor.
/// The file descriptor could be a socket,
/// an eventfd, a timerfd, or a signalfd

#include <functional>
#include <memory>
#include "sys/epoll.h"
#include "noncopyable.h"
#include "Timestamp.h"

class EventLoop; //  因为没有定义对象 所以只声明一下即可
using std::function;


// channel 将感兴趣的事情注册给Poller，Poller将fd上发生的事情返回给channel，channel再调用相应的回调函数
// EventLoop 有多个Channel，一个Poller。这个Poller监听多个Channel
// Channel 封装了
    //  fd
    //  挂在epoll tree上的fd的事件 如fd的EPOLLIN、EPOLLOUT
    //  handler
// Channel 就是 之前 我用C写的那个single process reactor 的my_event
// core function : handleEvent() 
class Channel : noncopyable
{
public:
    //  用于处理事件的回调函数
    using EventCallback = function<void()>;
    //  处理读事件
    using ReadEventCallback = function<void(Timestamp)>;

public:
    Channel(EventLoop *loop, int fd);
    ~Channel();

    void handleEvent(const Timestamp& receiveTime);

    //  设置回调函数
    void setReadCallback(ReadEventCallback call) { readCallback_ = std::move(call); }
    void setWriteCallback(EventCallback call) { writeCallback_ = std::move(call); }
    void setCloseCallback(EventCallback call) { closeCallback_ = std::move(call); }
    void setErrorCallback(EventCallback call) { errorCallback_ = std::move(call); }

    //  1. 设置fd感兴趣的事件：读 / 写
    //  2. update 将本channel绑定到所属eventloop的poller上。 
    void enableReading(){ events_ |= kReadEvent; update(); }
    void disableReading() { events_ &= ~kReadEvent; update(); }
    void enableWriting() { events_ |= kWriteEvent; update(); }
    void disableWriting() { events_ &= ~kWriteEvent; update(); }
    void disableAll() { events_ = kNoneEvent; update(); }
    //  whether writing/reading is registered on epoll
    bool isWriting() { return events_ &= kWriteEvent; }
    bool isReading() { return events_ &= kReadEvent; }
    bool isNoneEvent() { return events_ == kNoneEvent; }

    //  tie this channel to the owner object managed by shared_ptr
    //  prevent the owner object being destroyed in handleEvent;
    //  通过weak_ptr tie_
    void tie(const std::shared_ptr<void> &);

    int fd() const { return fd_; }                  //  return 监听的fd
    int events() const { return events_; }          //  return fd注册到epoll的事件
    void set_revents(int revt) { revents_ = revt; } //  used by poller to tell the channel 发生了什么事件

    //  For Poller
    //  记录Channel的状态
    //  index 代表 本Channel封装的 fd-event的状态
        //  记录在Poller中 && 在epoll上被监听 ： added
        //  记录在Poller中 && 没在epoll上被监听 ：deleted
        //  没记录在Poller中 && 没在epoll上被监听 ： New
    int index() { return index_; }
    void set_index(int idx) { index_ = idx; }

    //  EventLoop 删除本Channel
    void remove();
private:

    void update();
    //  weak_ptr -> lock -> handler
    void handlerEventWithGuard(const Timestamp& receiveTime);

private:
    static const int kNoneEvent = 0;         //  有无事件
    static const int kReadEvent = EPOLLIN | EPOLLPRI;   //  读事件
    static const int kWriteEvent = EPOLLOUT; //  写事件

    EventLoop *loop_; //  EventLoop channel归属于哪个EventLoop 为了获取poller
    const int fd_;    //  fd
    int events_;      //  fd感兴趣的事件的集合（注册在epoll tree上要被监听的事件的集合）
    int revents_;     //  poller 实际监听到的事件
    int index_;

    //  这个weak_ptr到底是引用谁？？？
    std::weak_ptr<void> tie_; //  weak_ptr 解决shared_ptr循环引用 以及 多线程安全问题
    bool tied_;               //  是否绑定了？话说绑定的到底是什么啊md

    //  因为Channel通道里面能够获知fd最终发生的具体的事件revents，所以它负责调用具体事件的回调操作
    ReadEventCallback readCallback_; //  读回调
    EventCallback writeCallback_;    //  写回调
    EventCallback closeCallback_;    //  连接关闭回调
    EventCallback errorCallback_;    //  error回调
};

#endif