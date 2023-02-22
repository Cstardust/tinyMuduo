#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<libgen.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<cassert>
#include<fcntl.h>
#include<unistd.h>
#include<iostream>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

using std::cout;
using std::endl;

//  epoll_ctl 将要监听的fd的event事件注册到epfd的事件表上
    //  将要监听的fd的EPOLLIN事件注册到事件表上
        //  事件相关信息(fd,EPOLLIN) 在struct epoll_event上
void addfd(int epfd,int fd,bool is_et);


int setnonblocking(int fd);

void lt(epoll_event *events,int num,int epfd,int lfd)
{
    char buf[BUFFER_SIZE]={0};
    for(int i=0;i<num;++i)
    {
        int fd = events[i].data.fd;
        if(fd == lfd)
        {
            struct sockaddr_in cliet_addr;
            socklen_t len = sizeof(cliet_addr);
            //  5.0  accept接收连接                        
            int connfd = accept(fd,(sockaddr*)&cliet_addr,&len);
            //  6  epoll_ctl 注册connfd到事件表 
            addfd(epfd,connfd,false);

            //  print user ip and port(from network to host)
            char remote[INET_ADDRSTRLEN];
            printf("connected with ip : %s and port %d\n",
                inet_ntop(AF_INET,&cliet_addr.sin_addr,remote,INET_ADDRSTRLEN),
                ntohs(cliet_addr.sin_port));
        }
        else if(events[i].events & EPOLLIN)
        {
            //  5.1 read
            //  只要socket读缓存中还有没读出的数据，这段代码就被触发。
            cout<<"event trigger once\n"<<endl;
            memset(buf,0,BUFFER_SIZE);
            int len = read(fd,buf,BUFFER_SIZE-1);   //  留一个位置server自己放0 因为我们需要打印 且 因为对方不一定发送\0作为结尾。lt模式下，只读一次，就不需要fd一定non blocking了。反正就读一次，EPOLLIN发生了就一定有数据，read就一定会返回。但是et模式下就不行，因为et模式必须一次循环读完所有数据，必然会遇到读尽数据的情况，如果fd是blocking的话，那么必然会阻塞在那里。
            if(len <= 0)                            
            {
                close(fd);
                continue;
            }
            cout<<"get "<<len<<" byte of content "<<buf<<endl;
        }
        else
        {
            printf("sth else happened\n");
        }
    }
}



void et(epoll_event *events,int num,int epfd,int lfd)
{
    char buf[BUFFER_SIZE];
    for(int i=0;i<num;++i)
    {
        int fd = events[i].data.fd;
        //  epoll监听到server的listening_fd发生事件
            //  有client连接，accept接受client连接，返回connfd，并epoll_ctl将connfd注册到epoll上。
        if(fd == lfd)
        {
            sockaddr_in cliet_addr;
            socklen_t cliet_len = sizeof cliet_addr;
            int connfd = accept(lfd,(sockaddr*)&cliet_addr,&cliet_len);
            addfd(epfd,connfd,true);

            char remote[INET_ADDRSTRLEN];
            printf("connected with ip : %s and port %d\n",
                inet_ntop(AF_INET,&cliet_addr.sin_addr,remote,INET_ADDRSTRLEN),
                ntohs(cliet_addr.sin_port));   
        }
        //  epoll监听到server的某个connfd发生事件
            //  client向server发送消息 server 从connfd中read出发过来的data
        else if(events[i].events & EPOLLIN)
        {
            memset(buf,0,sizeof buf);
            int len = 0;
            while((len = read(fd,buf,BUFFER_SIZE-1))>0)
            {
                buf[len] = '\0';
                cout<<"get "<<len<<" byte of content "<<buf<<endl;
            }          
            //  non blocking socket 无数据  
            if(len == -1 && errno == EAGAIN)    //  #define	EWOULDBLOCK	EAGAIN	/* Operation would block */
            {
                cout<<"read later"<<endl;
            }
            //  对端关闭
            else if(len == 0)
            {
                cout<<fd<< " peer is closed"<<endl;
                close(fd);
            }
        }
        else
        {
            cout<<"sth else happened"<<endl;
        }
    }
}


int main(int argc,char *argv[])
{
    if(argc <= 2)
    {
        printf("usage: %s ip_address port number\n",basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    short port = atoi(argv[2]);
    //  1. create socket
    int lfd = socket(PF_INET,SOCK_STREAM,0);
    assert(lfd >= 0);

    
    //  2. 将protocol、ip、port以网络字节序存储入server_addr
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof server_addr);
    server_addr.sin_family = AF_INET;                   //  protocol    network
    inet_pton(AF_INET,ip,&server_addr.sin_addr.s_addr); //  ip          str->network
    server_addr.sin_port = htons(port);                 //  port        network   
    //  bind socket with addr
    int ret = bind(lfd,(sockaddr*)&server_addr,sizeof server_addr);
    assert(ret!=-1);

    //  3. listen
    ret = listen(lfd,5);
    assert(ret!=-1);

    //  4.1 epoll create    
    int epfd = epoll_create(5);
    assert(epfd!=-1);
    epoll_event events[MAX_EVENT_NUMBER];   
    //  4.1.1 epoll_ctl 监听listening fd 以接收client connection
    addfd(epfd,lfd,true);

    while(1)
    {
        //  4.2 epoll_wait
        int events_num = epoll_wait(epfd,events,MAX_EVENT_NUMBER,-1);
        if(events_num == -1) 
        {
            printf("epoll failure\n");
            break;
        }
        //  deal with events monitored
        // lt(events,events_num,epfd,lfd);
        et(events,events_num,epfd,lfd);
    }    

    close(lfd);
    exit(0);
}   


int setnonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

// struct epoll_event
// {
//   uint32_t events;	/* Epoll events */
//   epoll_data_t data;	/* User data variable */
// } __EPOLL_PACKED;
//  将要监听的fd的EPOLLIN事件注册到事件表上
//  事件相关信息(fd,EPOLLIN) 在epoll_event上
void addfd(int epfd,int fd,bool is_et)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if(is_et)
    {
        event.events |= EPOLLET;
    }
    setnonblocking(fd);
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    //  将要监听的fd的event事件注册到epfd的事件表上
}
