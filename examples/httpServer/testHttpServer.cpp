#include <tinyMuduo/HttpServer.h>
#include <tinyMuduo/HttpRequest.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <cassert>

#define BUFFER_SIZE 1024


char hello_buf[BUFFER_SIZE]={0};

void readFile(const string &file)
{
    int fd = open(file.c_str(),O_RDONLY);
    int n = read(fd,hello_buf,1024);                                                                                             
    assert(n!=-1);
}

void respond(const HttpRequest& req, HttpResponse* resp)
{
  // hello.txt ok
  if (req.path() == "/hello.txt")   
  {
    //  状态行
    resp->setStatusCode(HttpResponse::k200Ok);    
    resp->setStatusMessage("OK");
    //  首部行
    resp->setContentType("text/plain");
    resp->addHeader("Server", "HttpServer");
    //  实体体
    resp->setBody(hello_buf);
  }
  // url指向资源不存在 , 404
  else  
  {
    //  状态行
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    //  首部行
    resp->setCloseConnection(true);
    //  实体体 : 空
  }
}


int main(int argc, char* argv[])
{
  readFile("./hello.txt");
  EventLoop loop;
  HttpServer hs(&loop, InetAddress(6667), "HttpServer");
  hs.setHttpCallback(respond);
  hs.setSubThreadNum(1);
  hs.start();
  loop.loop();
}
