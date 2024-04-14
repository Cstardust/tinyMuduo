#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

// #include "TcpServer.h"
// #include "noncopyable.h"
// #include "Logger.h"
// #include <string>
// #include "HttpResponse.h"
// #include "HttpRequest.h"

#include "TcpServer.h"
#include "noncopyable.h"
#include "Logger.h"
#include <string>
#include "HttpResponse.h"
#include "HttpRequest.h"

using namespace httpparser;


class HttpServer : noncopyable
{
public:
    using HttpCallback = std::function<void(const HttpRequest &, HttpResponse *)>;

    HttpServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const std::string &name);

    void setHttpCallback(const HttpCallback &cb) { httpCallback_ = cb; }
    void start();
    void setSubThreadNum(int num) { server_.setThreadNum(num); }

private:
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime);
    void onRequest(const TcpConnectionPtr &, const HttpRequest &);
    void defaultHttpCallback(const HttpRequest&, HttpResponse* resp);

    TcpServer server_;
    HttpCallback httpCallback_;
};

#endif // HTTP_HTTPSERVER_H