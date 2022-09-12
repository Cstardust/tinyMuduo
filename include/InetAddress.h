#ifndef MY_MUDUO_INET_ADDRESS
#define MY_MUDUO_INET_ADDRESS

// #include<arpa/inet.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"public.h"

//  封装了IPv4的socket地址
    //  构造时 传入 本地字节序,点分十进制的IP("127.0.0.1") 和 本地字节序的Port
    //  将ip和port以网络字节序俩保存进sockaddr_in
    //  并提供了两种接口 
        //  1. 返回 本地字节序IP和Port -- 提供给人类阅读
        //  2. 获取 socket地址结构（网络字节序保存了IP和Port）--- 提供给socket编程接口 
class InetAddress
{
public:
    explicit InetAddress(uint16_t port,const string &ip="127.0.0.1");
    explicit InetAddress(const sockaddr_in & addr);
    //  网络字节序->本地字节序 并返回本地字节序的IP和Port
    string toIp() const;
    uint16_t toPort() const;
    string toIpPort() const;
    //  返回sockaddr_in结构体
    const sockaddr_in *getSockAddr() const{return &addr_;}
private:
    //  sockaddr_in结构体以 **网络字节序** 保存 IP和Port
    sockaddr_in addr_;
    //  sin_family = AF_INET                        地址族
    //  uint16_t sin_port                           网络字节序的port
    //  struct in_addr sin_addr(uin32_t s_addr)     网络字节序的ip
};


#endif