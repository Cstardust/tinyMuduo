#include<tinyMuduo/Buffer.h>
#include<tinyMuduo/Logger.h>
#include<unistd.h>
#include<fcntl.h>
#include<cstring>

//  test buffer
int Open(string path,int flag)
{
    int fd = open(path.c_str(),O_RDONLY);
    if(fd==-1)
    {
        LOG_ERROR("errno = %d ; error = %s\n",errno,strerror(errno));
    }
    return fd;
}

int main()
{
    int fd = Open("./BufferTest.txt",O_RDONLY);
    int err = 0;
    Buffer buffer;              //  初始化
    
    //  读取数据到buffer
    buffer.readFd(fd,&err);     //  扩容
    
    #define TEST_RETRIEVE
    
    #ifdef TEST_RETRIEVE
    
    //  test1 : 读出buffer中所有数据
    string str = buffer.retrieveAllAsString();   //  回退 readerIdx writeIdx   
    cout<<str.size()<<endl;

    #else 
    //  test2 : buffer将数据写出. 写到终端
    size_t n = buffer.writeFd(STDOUT_FILENO,&err);
    #endif

    return 0;
}