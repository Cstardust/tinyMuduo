#include"InetAddress.h"
#include<cstring>

InetAddress::InetAddress(uint16_t port,const string &ip/*="127.0.0.1"*/)
{
    memset(&addr_,0,sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);                   //  save host port as network port (convert host to network short) 
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());  //  save string ip as network ip
        //  inet_addr : 将点分十进制字符串表示的IPV4地址转化为用网络字节序表示的IPV4地址
}



InetAddress::InetAddress(const sockaddr_in& addr)
    :addr_(addr)
    {}

//  返回本地字节序 电分十进制表示的IPV4地址
string InetAddress::toIp() const
{
    return inet_ntoa(addr_.sin_addr);
}

// 返回本地字节序的port
uint16_t InetAddress::toPort() const
{
    return ntohs(addr_.sin_port);
}

string InetAddress::toIpPort() const
{
    char host_str_ip_port[64]={0};
    //  convert network int ip to host string ip
    inet_ntop(AF_INET,&addr_.sin_addr,host_str_ip_port,sizeof host_str_ip_port);
    int len = strlen(host_str_ip_port);
    //  convert network port to host port
    uint16_t host_port = ntohs(addr_.sin_port);
    //  ip + port
    sprintf(host_str_ip_port+len,":%u",host_port);
    return host_str_ip_port;
}
