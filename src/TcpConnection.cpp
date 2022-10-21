#include "TcpConnection.h"
#include "Logger.h"
#include "Socket.h"
#include "Channel.h"
#include "EventLoop.h"
#include<cassert>

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
                             const InetAddress &peerAddr)
    : loop_(checkLoopNotNull(loop)),
      name_(name),
      state_(0),
      reading_(false),
      socket_(new Socket(connfd)),
      channel_(new Channel(loop,connfd)),
      localAddr_(localAddr),
      peerAddr_(peerAddr),
      highWaterMark_(64*1024*1024)      //  64 MB
{
    //  channel设置handler
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead,this,std::placeholders::_1));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose,this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError,this));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite,this));


    LOG_INFO("TcpConnection::ctor [%s] on fd = %d\n",name_.c_str(),connfd);
    socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_INFO("TcpConnection::ctor (%p) [%s] on socket %d with state %d\n",this,name_.c_str(),socket_->fd(),state_.load());
}

//  注册给channel 这个函数最终会被ioloop调用
void TcpConnection::handleRead(Timestamp receiveTime)
{
    int savedErrno = 0;
    //  将fd中可读的数据 读到inputBuffer中
    ssize_t n = inputBuffer_.readFd(channel_->fd(),&savedErrno);

    LOG_INFO("handle read %lu bytes",n);
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


//  outputBuffer中有未发送数据 且 tcp socket的写缓冲区可写的时候 loop会执行这个call back
void TcpConnection::handleWrite()
{
    LOG_INFO("fd = %d , outputBuffer data size = %ld ",channel_->fd(),outputBuffer_.readableBytes());
    //  channel_ 封装的fd 有注册在epoll上的writing事件
    if(channel_->isWriting())
    {
        int savedErrno = 0;
        //  outputBuffer中的bytes 写入 可写的fd
        ssize_t n = outputBuffer_.writeFd(channel_->fd(),&savedErrno);
        if(n > 0)
        {
            //  如果已经没有数据可以从buffer中取出  可取bytes : [readerIdx,writerIdx) 
                //  即buffer中的数据都已经写给fd
                //  那么我们需要立刻停止epoll对writable事件的监听
            //  !=0的话 代表buffer中还有可写数据 那么不必取消监听。epoll稍后应当检测到可写事件 然后继续调用本函数进行写
            if(outputBuffer_.readableBytes() == 0)
            {
                //  取消监听！！！
                channel_->disableWriting();
                //  执行写完成回调
                if(writeCompleteCallback_)
                {
                    //  唤醒loop对应的thread线程 执行回调
                        //  这个writeCompleteCallback 是什么？
                    loop_->queueInLoop(
                        std::bind(writeCompleteCallback_,shared_from_this())
                    );
                }
                //  如果全部outputBuffer里面的数据全部写出 并且server已经要求关闭这个连接，但是之前由于outputBuffer里面有要写的数据 因此没有关闭连接 的话
                //  那么 此时 数据全部写出 就要调用刚才没执行的shutdownInLoop()
                if(state_ == kDisconnecting)
                {
                    //  关闭socket的write方向
                    shutdownInLoop();
                }
            }
            //  如何做到一个事件的处理不长时间占用控制权？措施之一如下
                //  如果一次没写完的话，不会阻塞在这里等待缓冲区空闲然后继续写
                //  而是将控制交还给eventloop。因为写事件还在epoll tree上监听，并且未写的数据存在outputBuffer中
                //  等待下一次循环poller监听到写事件 触发write即可。!!!
            else
            {
                LOG_INFO("I'm going to write more data");
            }
        }
        else
        {
            LOG_ERROR("TcpConnection : outputBuffer write error\n");
        }
    }
    else
    {
        LOG_ERROR("TcpConnection fd = %d is down , no more is writing\n",channel_->fd());
    }
}


//  我应该是写错了什么地方 不然不可能析构。。下午/晚上再排查吧。。
    //  破案了 传参的时候传错了。应当传shared_ptr 而非裸指针
//  poller ->  channel::closeCallback -> TcpConnection::handleClose -> TcpServer::removeConnection -> TcpConnection::connectionDestoyed
void TcpConnection::handleClose()
{
    LOG_INFO("TcpConnection::handleClose fd = %d state = %d\n",socket_->fd(),state_.load());

    //  调用handleClose的情况
        //  写端 读端都没关闭 kConnected : 调用handleClose 
        //  写端关闭 kDisconnecting : 调用handleClose
    assert(state_ == kConnected || state_ == kDisconnecting);
    //  改变状态
    setState(kDisconnected);
    //  从epoll树上拿下来
    channel_->disableAll();

    // LOG_INFO("shared_ptr cnt %ld",shared_from_this().use_count());
    TcpConnectionPtr connPtr(shared_from_this());
    // LOG_INFO("shared_ptr cnt %ld",connPtr.use_count());
    
    //  user设置的
    connectionCallback_(connPtr);       //  连接断开和连接的回调

    // LOG_INFO("shared_ptr cnt %ld",connPtr.use_count());

    //  不是user设置的
    //  TcpServer::removeConnection -> TcpConnection::connectionDestroyed
    closeCallback_(connPtr);            //  关闭连接的回调  TcpServer::removeConnection

    //  记得研究tcpConnectionPtr计数
    LOG_INFO("shared_ptr cnt %ld tcpconnection %p point to %p",connPtr.use_count(),connPtr.get(),this);
}


void TcpConnection::handleError()
{
    int optval;
    socklen_t optlen = sizeof optval;
    int err = 0;
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

//  send -> sendInLoop ->注册handleWrite
void TcpConnection::send(const std::string &buf)
{
    // LOG_INFO("send bytes buf %s",buf.c_str());
    int n = buf.size();
    const char *str = buf.c_str();
    LOG_INFO("test send %d : %s",n,str);
    if(state_ == kConnected)
    {
        //  sendInLoop
            //  会根据 output queue中是否还有未发送数据
            //  来决定是直接发送buf 还是先将buf送入outputBuffer 再发送
        if(loop_->isInLoopThread())
        {
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

    //  如果output queue(outputBuffer_) 中没有任何数据的话，那么就直接发送给fd。
        //  表示channel上并没有写事件被监听，而且缓冲区没有待发送数据
    if(!channel_->isWriting() && outputBuffer_.readableBytes()==0)
    {
        LOG_INFO("直接将data发送，而不是先送入outputBuffer");
        //  output queue中没有待发送数据 因此 直接将data写给fd
        nwrote = ::write(channel_->fd(),data,len);
        LOG_INFO("nwrote = %d",nwrote);
        if(nwrote >= 0)
        {
            //  是否将data都发送完
            remaning = len - nwrote;
            //  我猜这个writeCompleteCallback 是用户设置的
            if(remaning == 0 && writeCompleteCallback_)
            {
                //  如果发送完了 那么就调用一下writeCompleteCallback_
                //  既然在这里 数据全部发送完成 也就无需给channel 设置 epollout事件了
                loop_->queueInLoop(
                    std::bind(writeCompleteCallback_,shared_from_this())
                );
                //  一次全部写完 结束 无需后续监听写事件/加入outputBuffer
                return ;
            }
            //  没发送完 继续发送
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
                    LOG_INFO("errno == %s",strerror(errno));
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
        LOG_INFO("data没能全部发送走 因此将剩余的data放入outputBuffer");
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

        //  将data中没有发送的数据 存入outputBuffer
        outputBuffer_.append((char*)data+nwrote,remaning);
        //  这里一定要注册Channel的写事件
        //  否则poller不会给channel通知epollout
        if(!channel_->isWriting())
        {
            LOG_INFO("将fd = %d的写事件 注册给poller",channel_->fd());
            channel_->enableWriting();
        }
    }
}


// 连接建立
    //  调用之前TcpServer注册的 connectionCallback_ (即TcpServer::connectionCallback_ )  Tcp::connectionCallback其实也是user set的
    //  eventloop 会调用该callback
void TcpConnection::connectionEstablished()
{
    LOG_INFO("connectionEstablised in loop %p on thread %d",loop_,CurrentThread::gettid());
    setState(kConnected);
    //  延长channel生命周期 ？
    channel_->tie(shared_from_this());
    // 给新建立的connfd 注册读事件到Poller
    channel_->enableReading();           

    // 新连接建立，执行回调
    if(connectionCallback_)
    {
        //  user set the connectionCallback
        connectionCallback_(shared_from_this());
    }
    else
    {
        LOG_INFO("user didn't set connectionCallback ; connectionCallback is nullptr");
    }
}

// 连接销毁
void TcpConnection::connectionDestroyed()
{
    LOG_INFO("state_ = %d",state_.load());
    //  正常应该是先handleClose 之后再connectionDestroyed 。state再handleClose里面状态就已经改变了
        // 这样就进不来 因为之前会调用connection
    //  还有一种情况就是直接调用connectionDestroyed 如在TcpServer的析构函数中
        //  防止直接调用connectionDestroyed
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
    LOG_INFO("shutdown in thread %d",CurrentThread::gettid());
    //  已连接
    if(state_ == kConnected)        
    {
        setState(kDisconnecting);   //  正在关闭
        loop_->runInLoop(
            std::bind(&TcpConnection::shutdownInLoop,this)
        );
    }
}

//  保证在IO thread中调用
//  当outputBuffer里面没有待发送数据时 关闭本connection的socket写端
//  当outputBuffer里面有待发送数据时   等handleWrite将outputBuffer全部写出之后 handleWrite会调用shutdownInLoop
void TcpConnection::shutdownInLoop()
{
    //  outputBuffer中的bytes已经全部发送完成
    if(!channel_->isWriting())          
    {
        socket_->shutdownWrite();       //  关闭写端
    }
    //  如果outputBuffer没全部发送
        //  之后也会调用到shutdownInLoop
        //  见handleWrite
}