#include "Buffer.h"
#include<unistd.h>
#include<sys/uio.h>

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

    const int iovcnt = (writable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = ::readv(fd,vec,iovcnt);
    if(n<0)
    {
        *saveErrno = errno;
    }
    //  Buffer可写缓冲区够存储读出来的数据
    else if(n <= writable)
    {
        writerIdx_ += n;
    }
    //  extrabuf里面也写入了数据
    else
    {
        //  writeIdx开始写 n-writable大小的数据
        //  ???? 不就是缓冲区空间不够了 才写到buffer里的吗？
        //  怎么还要再写回去？
            //  因为append里面会扩容 保证一定可以写下
        writerIdx_ = buffer_.size();
        append(extrabuf,n-writable);
    }

}



ssize_t Buffer::writeFd(int fd,int *saveErrno)
{
    //  将可读数据写给fd????没搞错吧？？？
    //  不应该是将写入缓冲区的数据写给fd吗???
    ssize_t n = ::write(fd,peek(),readableBytes());
    if(n < 0)
    {
        *saveErrno = errno;
    }
    return n;
}