#ifndef _CAT_SERVER_
#define _CAT_SERVER_

#include "public.h"
#include <tinyMuduo/TcpServer.h>

//  网络模块
class CatServer
{
public:
    CatServer(EventLoop *loop,
              const InetAddress &addr,
              const string &name);

    void setSubThreadNum(int n);
    void start();

private:
    //  设置callback 注册给reactor中的Eventloop
        //  连接 / 断开 时调用
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr & conn , Buffer *buffer,Timestamp time);
    TcpServer server_;
};

#endif