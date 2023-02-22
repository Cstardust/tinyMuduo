#include "HttpServer.h"
#include "HttpContext.h"

void HttpServer::defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
    // default http callback
    // status code 404
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
}

HttpServer::HttpServer(EventLoop *loop,
                      const InetAddress &listenAddr,
                      const std::string &name)
  : server_(loop, listenAddr, name),
    httpCallback_(std::bind(&HttpServer::defaultHttpCallback,this,std::placeholders::_1, std::placeholders::_2))
{
    server_.setConnectionCallback(
        std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
    server_.setMessageCallback(
        std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void HttpServer::start()
{
    server_.start();
}

void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
    if(conn->connected())
    {
        LOG_INFO("connection up : from %s to %s!",conn->peerAddress().toIpPort().c_str(),conn->localAddress().toIpPort().c_str());
    }
    else
    {
        LOG_INFO("connection down : from %s to %s!",conn->peerAddress().toIpPort().c_str(),conn->localAddress().toIpPort().c_str());
    }
}

// 有消息到来时的业务处理
void HttpServer::onMessage(const TcpConnectionPtr& conn,
                           Buffer* buf,
                           Timestamp receiveTime)
{
    HttpContext context;
#if 0
    // echo
    std::string request = buf->retrieveAllAsString();
    std::cout << request << std::endl;
#else
    // FSM 解析
    //  解析error
    if (!context.parseRequest(buf, receiveTime))
    {
        buf->clear();
        LOG_ERROR ("parseRequest failed due to invalid http request");
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }

    // 解析successfully
    if (context.gotAll())
    {
        LOG_DEBUG ("parseRequest success!");
        onRequest(conn, context.request());
        context.reset();
    }
#endif
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req)
{
    // 长连接 or 短连接
    //  HTTP/1.0 的默认模型就是短连接（如果没有指定 Connection 协议头，或者是Connection值被设置为 close)
    //  而在 HTTP/1.1 中，只有当 Connection 被设置为 close 时才会用到这个模型。
    //  HTTP/1.1 里，默认就是长连接的，不再需要标头
    const std::string& connection = req.getHeader("Connection");    //  empty or close or Keep-Alive
    bool close = connection == "close" || (req.version() == HttpRequest::kHttp10 && connection != "Keep-Alive");
    
    //  response
    HttpResponse response(close);
    
    //  httpCallback_ 由class httpServer的使用者设计传入. 
    //  httpCallback_接收刚才解析完毕的httpRequest
    //  httpCallback_判断请求是否成功 并返回相应资源
    httpCallback_(req, &response);
    
    Buffer buf;
    response.appendToBuffer(&buf);
    conn->send(buf.retrieveAllAsString());
    
    //   短连接 , 关闭
    if (response.closeConnection())
    {
        conn->shutdown();
    }
}