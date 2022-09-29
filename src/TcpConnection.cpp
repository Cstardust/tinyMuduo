#include "TcpConnection.h"
#include "Logger.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"


static EventLoop *checkLoopNotNull(EventLoop *loop)
{
    if (loop == nullptr)
    {
        LOG_FATAL("TcpConnection Loop is null!\n");
    }
    return loop;
}

TcpConnection::TcpConnection(EventLoop *loop,
                             const std::string &name,
                             int connfd,
                             const InetAddress &localAddr,
                             const InetAddress &clietAddr)
    : loop_(checkLoopNotNull(loop)),
      name_(name),
      state_(0),
      reading_(false),
      socket_(new Socket(connfd)),
      channel_(new Channel(loop,connfd)),
      localAddr_(localAddr),
      clietAddr_(clietAddr),
      highWaterMark_(64*1024*1024)      //  64 MB
{
    //  channel设置handler
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead,this,std::placeholders::_1));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose,this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError,this));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite,this));

    LOG_INFO("TcpConnection::ctor [%s] on fd = %d\n",name_.c_str(),connfd);
    //  作用？？？
    socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_INFO("TcpConnection::ctor [%s] on socket %d with state %d\n",name_.c_str(),socket_->fd(),state_.load());
}


void TcpConnection::handleRead(Timestamp receiveTime)
{
    int savedErrno = 0;
    //  从缓冲区中读数据
    ssize_t n = inputBuffer_.readFd(channel_->fd(),&savedErrno);
    if(n>0)
    {
        messageCallback_(shared_from_this(),&inputBuffer_,receiveTime);
    }
    else if(n == 0)
    {
        handleClose();
    }
    else
    {
        errno = savedErrno;
        LOG_ERROR("TcpConnection::handleRead");
        handleError();
    }
}






//  没太懂 先写上吧。
void TcpConnection::handleWrite()
{
    //  channel_ 封装的fd 有注册在epoll上的writing事件
    if(channel_->isWriting())
    {
        int savedErrno = 0;
        //  写缓冲区中可以写给fd的字节数量
        ssize_t n = outputBuffer_.writeFd(channel_->fd(),&savedErrno);
        if(n > 0)
        {
            //  没太理解？？？？
            //  取出一部分可以读走的字节
            outputBuffer_.retrieve(n);
            //  一定就是0吗？不是0就不读了？？
            if(outputBuffer_.readableBytes() == 0)
            {
                //  不再监听写事件？？为什么？
                    //  因为将outputBuffer中的可读数据都读走了?
                channel_->disableWriting();
                if(writeCompleteCallback_)
                {
                    //  唤醒loop对应的thread线程 执行回调
                        //  这个writeCompleteCallback 是什么？
                    loop_->queueInLoop(
                        std::bind(writeCompleteCallback_,shared_from_this())
                    );
                }
                if(state_ == kDisconnecting)
                {
                    shutdownInLoop();
                }
            }
        }
        else
        {
            LOG_ERROR("TcpConnection : handlerWrite\n");
        }
    }
    else
    {
        LOG_ERROR("TcpConnection fd = %d is down , no more is writing\n",channel_->fd());
    }
}



void TcpConnection::handleClose()
{
    LOG_INFO("TcpConnection::handleCode fd = %d state = %d\n",socket_->fd(),state_.load());

    setState(kDisconnected);
    channel_->disableAll();

    TcpConnectionPtr connPtr(shared_from_this());
    connectionCallback_(connPtr);       //  连接关闭的回调
    closeCallback_(connPtr);            //  关闭连接的回调
}


void TcpConnection::handleError()
{
    int optval;
    socklen_t optlen = sizeof optval;
    int err = 0;
    //  干嘛？？
    if(::getsockopt(channel_->fd(),SOL_SOCKET,SO_ERROR,&optval,&optlen) < 0)
    {
        err = errno;
    }
    else
    {
        err = optval;
    }
    LOG_ERROR("TcpConnection::handlerError name : %s - SO_ERROR:%d\n",name_.c_str(),err);
}


void TcpConnection::send(const std::string &buf)
{
    if(state_ == kConnected)
    {
        if(loop_->isInLoopThread())
        {
            //  send in loop 不会改变buf吗
            sendInLoop(buf.c_str(),buf.size());
        }
        else
        {
            loop_->runInLoop(std::bind(
                &TcpConnection::sendInLoop,
                this,
                buf.c_str(),
                buf.size()
            ));
        }
    }
}

/*
发送数据：
    应用写的快 而内核发送数据慢
    需要把待发送的数据写入缓冲区
    而且设置了水位回调
*/
void TcpConnection::sendInLoop(const void *data,size_t len)
{
    ssize_t nwrote = 0;
    size_t remaning = len;
    bool faultError = false;

    //  之前调用过该connection的shutdown 不能再进行发送了
    if(state_ == kDisconnected)
    {
        LOG_ERROR("disconnected give up writing!");
        return ;
    }

    //  表示channel第一次开始写数据 而且缓冲区没有待发送数据
    if(!channel_->isWriting() && outputBuffer_.readableBytes()==0)
    {
        nwrote = ::write(channel_->fd(),data,len);
        if(nwrote >= 0)
        {
            remaning = len - nwrote;
            if(remaning == 0 && writeCompleteCallback_)
            {
                //  既然在这里 数据全部发送完成 就不用再给channel 设置 epollout事件了
                loop_->queueInLoop(
                    std::bind(writeCompleteCallback_,shared_from_this())
                );
            }
        }
        else    //  nwrote < 0
        {
            nwrote = 0;
            if(errno != EWOULDBLOCK)
            {
                LOG_ERROR("TcpConnection::sendInLoop error");
                //  对方socket重置
                    //  server收到
                    //  SIGPIPE
                    //  RESET
                if(errno == EPIPE || errno == ECONNRESET)
                {
                    faultError = true;
                }
            }
        }
    }

    //  说明当前这一次write并没有把数据全部发送出去
    //  剩余的数据需要保存到缓冲区当中 
    //  然后给channel注册epollout事件
    //  poller发现tcp的发送缓冲区有空间
    //  会通知相应的sock->channel
    //  调用writeCallback_ 回调方法
    //  也就是TcpConnection::handleWrite方法 把发送缓冲区的数据全部发送完成为止
    if(!faultError && remaning > 0)
    {
        //  目前发送缓冲区剩余的待发送数据的长度
        size_t oldlen = outputBuffer_.readableBytes();
        if(oldlen + remaning >= highWaterMark_
            && oldlen < highWaterMark_
            && highWaterMarkCallback_)
        {
            loop_->queueInLoop(
                std::bind(highWaterMarkCallback_,shared_from_this(),oldlen + remaning)
            );   
        }
        outputBuffer_.append((char*)data+nwrote,remaning);

        //  这里一定要注册Channel的写事件
        //  否则poller不会给channel通知epollout
        if(!channel_->isWriting())
        {
            channel_->enableReading();
        }
    }
}



// 连接建立
void TcpConnection::connectionEstablished()
{
    setState(kConnected);
    channel_->tie(shared_from_this());
    channel_->enableReading(); // 向poller注册channel的epollin事件

    // 新连接建立，执行回调
    connectionCallback_(shared_from_this());
}

// 连接销毁
void TcpConnection::connectionDestroyed()
{
    if (state_ == kConnected)
    {
        setState(kDisconnected);
        channel_->disableAll(); // 把channel的所有感兴趣的事件，从poller中del掉
        connectionCallback_(shared_from_this());
    }
    channel_->remove(); // 把channel从poller中删除掉
}


//  关闭连接
void TcpConnection::shutdown()
{
    if(state_ == kConnected)
    {
        setState(kDisconnecting);
        loop_->runInLoop(
            std::bind(&TcpConnection::shutdownInLoop,this)
        );
    }
}

//  
void TcpConnection::shutdownInLoop()
{
    if(!channel_->isWriting())          //  说明outputBuffer已经全部发送完成
    {
        socket_->shutdownWrite();       //  关闭写端
    }
}