#include "Buffer.h"
#include<unistd.h>
#include<sys/uio.h>
#include"Logger.h"

//  readv
ssize_t Buffer::readFd(int fd,int *saveErrno)
{
    //  stack上的内存空间 64K
    char extrabuf[65536] = {0};

    //  buffer底层缓冲区剩余的可写空间的大小
    const size_t writable = writableBytes();
    //  iovec
    struct iovec vec[2];
        //  可写缓冲区
    vec[0].iov_base = begin() + writerIdx_;
    vec[0].iov_len = writable;
        //  多的字节放入buf这里
    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;
    
    //  ? ??
    // when there is enough space in this buffer, don't read into extrabuf.
    // when extrabuf is used, we read 128k-1 bytes at most.
    const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    
    const ssize_t n = ::readv(fd,vec,iovcnt);

    LOG_INFO("buffer writable = %lu ; iovcnt = %d ; read n = %ld\n",writable,iovcnt,n);


    if(n<0)
    {
        *saveErrno = errno;
    }
    //  Buffer的[writerIdx,size) 足够存储读入的nbytes
    //  writerIdx移动n就好
    else if(n <= writable)
    {
        writerIdx_ += n;
    }
    //  extrabuf里面也写入了数据
    //  将读入extrabuf的字节存入buffer
    else
    {
        //  writeIdx开始写 n-writable大小的数据
        //  ???? 不就是缓冲区空间不够了 才写到buffer里的吗？
        //  怎么还要再写回去？
            //  因为append里面会扩容 保证一定可以写下
        //  buffer的writerIdx之后的空间已经被readv写满了
            //  故移动buffer
        writerIdx_ = buffer_.size();
        //  将读入extrabuf中的数据写入buffer。
        append(extrabuf,n-writable);
    }

    LOG_INFO("read finish : readerIdx = %ld ; writerIdx = %ld; readableBytes = %ld ; total size = %lu\n",readerIdx_,writerIdx_,readableBytes(),buffer_.size());

    return readableBytes();
}


//  write
//  将buffer中的可读数据 全部写给fd
    //  也即 将[readerIdx,writerIdx)的bytes 都写给fd
ssize_t Buffer::writeFd(int fd,int *saveErrno)
{
    ssize_t n = ::write(fd,peek(),readableBytes());
    if(n < 0)
    {
        *saveErrno = errno;
    }
    return n;
}