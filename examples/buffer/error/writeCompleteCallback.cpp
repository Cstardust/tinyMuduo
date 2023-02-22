#include<tinyMuduo/TcpServer.h>

class EchoServer
{
public:
    EchoServer(EventLoop *loop,const InetAddress& addr)
        :server_(loop,addr,"EchoServer")
    {
        server_.setConnectionCallback(std::bind(&EchoServer::onConnection,this,std::placeholders::_1));
        server_.setMessageCallback(std::bind(&EchoServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        server_.setWriteCompleteCallback(std::bind(&EchoServer::onWriteComplete,this,std::placeholders::_1));
    }

    void start()
    {
        server_.start();
    }

    void setThreadNum(int x)
    {
        server_.setThreadNum(x);
    }

private:
    void onConnection(const TcpConnectionPtr& conn)
    {
        if(conn->connected())
        {
            LOG_INFO("CONNECTION UP %s\n",conn->name().c_str());
        }
        else
        {
            LOG_INFO("CONNECTION DOWN %s\n",conn->name().c_str());
        }
    }

    void onMessage(const TcpConnectionPtr &conn,Buffer *buffer,Timestamp time)
    {
        LOG_INFO("onMessage\n");
        
        conn->send(buffer->retrieveAllAsString());

        // conn->shutdown();
    }

// void(const TcpConnectionPtr &)
    void onWriteComplete(const TcpConnectionPtr &conn)
    {
        //  as soon as send finished
        printf("be careful about stackoverflow\n");
        string msg("hope not stackoverflow!");
        conn->send(msg);
    }

private:
    TcpServer server_;
};


int main()
{
    EventLoop loop;
    InetAddress addr(6667);
    EchoServer es(&loop,addr);
    es.setThreadNum(4);
    es.start();       
    loop.loop();
}