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

/// Channel 封装了
//  fd
//  要监听的fd的事件 如fd的EPOLLIN、EPOLLOUT
//  事件发生后的相应的回调函数
//  channel 将感兴趣的事情注册给Poller，Poller将fd上发生的事情返回给channel，channel再调用相应的回调函数
/// EventLoop 有多个Channel，一个Poller。这个Poller监听多个Channel
/// EventLoop对应Reactor模型上的Demultiplex
class Channel : noncopybale
{
public:
    //  用于处理事件的回调函数
    using EventCallback = function<void()>;
    using ReadEventCallback = function<void(Timestamp)>;

    Channel(EventLoop *loop, int fd);
    ~Channel();

    void handleEvent(const Timestamp& receiveTime);

    //  设置回调函数（为什么要std move? 我觉得也没多大吧）
    void setReadCallback(ReadEventCallback call) { readCallback_ = std::move(call); }
    void setEventCallback(EventCallback call) { writeCallback_ = std::move(call); }
    void setCloseCallback(EventCallback call) { closeCallback_ = std::move(call); }
    void setErrorCallback(EventCallback call) { errorCallback_ = std::move(call); }

    //  设置fd感兴趣的事件：读 / 写
    void enableReading(){ events_ |= kReadEvent; update(); }
    void disableReading() { events_ &= ~kReadEvent; update(); }
    void enableWriting() { events_ |= kWriteEvent; update(); }
    void disableWriting() { events_ &= ~kReadEvent; update(); }
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
    // loop_->getPoller()->update();
    void update();
    //  weak_ptr -> lock -> handler
    void handlerEventWithGuard(const Timestamp& receiveTime);

private:
    static const int kNoneEvent = 0;         //  有无事件
    static const int kReadEvent = EPOLLIN | EPOLLPRI;   //  读事件
    static const int kWriteEvent = EPOLLOUT; //  写事件

    EventLoop *loop_; //  EventLoop channel归属于哪个EventLoop 为了获取poller
    const int fd_;    //  Poller的监听对象 epoll_ctl(fd?)
    int events_;      //  注册要监听的fd的事件？
    int revents_;     //  it's the received event types of epoll or poll
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