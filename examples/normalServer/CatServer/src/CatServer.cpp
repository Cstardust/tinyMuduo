#include "CatServer.h"
#include "json.hpp"
#include "CatService.h"

using nlohmann::json;

CatServer::CatServer(EventLoop *loop,
            const InetAddress &addr,
            const string &name)
    : server_(loop, addr, name)
{
    server_.setConnectionCallback(std::bind(&CatServer::onConnection,this,std::placeholders::_1));
    server_.setMessageCallback(std::bind(&CatServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
}


void CatServer::setSubThreadNum(int n)
{
    assert(n >= 0);
    server_.setThreadNum(n);
}

void CatServer::start()
{
    server_.start();
}

void CatServer::onConnection(const TcpConnectionPtr &conn)
{
    if(conn->connected())
    {
        LOG_INFO("connection up : from %s to %s ! \n",conn->peerAddress().toIpPort().c_str(),conn->localAddress().toIpPort().c_str());
    }
    else
    {
        LOG_INFO("connection down : from %s to %s ! \n",conn->peerAddress().toIpPort().c_str(),conn->localAddress().toIpPort().c_str());
    }
}


void CatServer::onMessage(const TcpConnectionPtr & conn , Buffer *buffer,Timestamp time)
{
    // LOG_INFO("on message \n");
    // conn->send(buffer->retrieveAllAsString());
    // conn->shutdown();      
    string msg(buffer->retrieveAllAsString());
    json js = json::parse(msg);     //  反序列化        //  确保msg是json，不然server会core down
    //  根据id回调相应的handler 而不是在这里switch_case
    //  通过map<id,handler> 实现网络模块和业务模块的解耦
    int msg_id = js["msg_id"].get<int>();

    CatService *service = CatService::getInstance();
    //  获取handler
    MsgHandler handler = service->getMsgHandler(MsgType(msg_id));
    handler(conn,js,time);
    return ;
}