#include<miniMuduo/TcpServer.h>
#include<functional>
#include<cassert>

//  user只需要自己编写当server接收到connection fd发送的数据时的动作
//  以及当server接收到新连接时 除了要accept连接之外的动作。accept的相关动作都已经由miniMuduo中的accepto实现了。
class EchoServer
{
public:
    EchoServer(EventLoop *loop,
                const InetAddress &addr,
                const std::string &name)
            :server_(loop,addr,name)
    {
        server_.setMessageCallback(std::bind(&EchoServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        server_.setConnectionCallback(std::bind(&EchoServer::onConnection,this,std::placeholders::_1));
    }

    void start()
    {
        server_.start();
    }

    void setSubThreadNum(int num)
    {
        assert(num >= 0);
        server_.setThreadNum(num);
    }
private:
    //  这些callback 最后应当都是在loop中调用
    //  连接建立完成 或者 连接断开完成后 调用user设定的onConnection
    void onConnection(const TcpConnectionPtr& conn)
    {
        if(conn->connected())
        {
            LOG_INFO("connection up : from %s to %s!\n",conn->peerAddress().toIpPort().c_str(),conn->localAddress().toIpPort().c_str());
        }
        else
        {
            LOG_INFO("connection down : from %s to %s!\n",conn->peerAddress().toIpPort().c_str(),conn->localAddress().toIpPort().c_str());
        }
    }

    //  当server收到消息时 该做什么
        //  这里传入给onMessage的buffer 就是TcpConnection中的inputBuffer
        //  inputBuffer中 存的就是server从client fd的socket kernel读缓冲区读取进内存的数据
        //  tcpconnection 如果要发送该读取到inputBuffer的数据的话
        //  那么就调用send(buffer->retrieveAllAsString()) （取出inputBuffer中的data，然后直接发送或者先存入outputBuffer再发送)
        //  tcpconnection inputBuffer读取 和 outputBuffer输出的对象 都固定了：一方是己方server 一方是连接来的client fd
    void onMessage(const TcpConnectionPtr& conn,Buffer *buffer,Timestamp time)
    {
        LOG_INFO("on Message");
        //  发送给client fd
            //  eventLoop -> send -> sendInLoop
        conn->send(buffer->retrieveAllAsString());
        //  关闭连接
            //  shutdown -> shutdownInLoop -> shutdownWrite
        conn->shutdown();
    }
private:
    TcpServer server_;
};


int main()
{
    LOG_INFO("main thread : %d\n",CurrentThread::gettid());
    //  在main thread创建的main loop。这个loop负责接收新连接
    //  user传入main loop
    EventLoop loop;
    InetAddress addr(6667);
    EchoServer es(&loop,addr,"EchoServer");
    //  设置subloop/thread的数量
    es.setSubThreadNum(3);
    //  开启server 开启监听 监听到的事件会通知给loop
    es.start();
    //  mainLoop的循环需要由user开启。其实感觉也可以设置成由start开启。。不过可能user在这之前还需要做其他动作？？
    loop.loop();
}