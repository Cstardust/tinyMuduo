#ifndef MY_MUDUO_BUFFER
#define MY_MUDUO_BUFFER

#include <vector>
#include <string>
#include <algorithm>
#include<errno.h>


//  Buffer未能完全理解

//  网络库底层缓冲区
class Buffer
{
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;

    explicit Buffer(size_t initialSize = kInitialSize)
        : buffer_(kCheapPrepend + initialSize),
          readerIdx_(kCheapPrepend),
          writerIdx_(kCheapPrepend)
    {
    }

    //  返回可读的字节数
    size_t readableBytes() const
    {
        return writerIdx_ - readerIdx_;
    }

    //  返回待写的字节数
    size_t writableBytes() const
    {
        return buffer_.size() - writerIdx_;
    }

    //  返回prependable 的字节数
    size_t prependableBytes() const
    {
        return readerIdx_;
    }

    //  返回缓冲区中可读数据的起始地址
    const char *peek() const
    {
        //  buffer起始地址 + 字节数 ?
        //  vector 底层是连续的
        return begin() + readerIdx_;
    }

    //  读走len长度的字节之后 移动readIdx_ ?
    //  on Message : string <- Buffer
    void retrieve(size_t len)
    {
        if (len < readableBytes())
        {
            //  应用只读取了可读缓冲区数据的一部分，就是len
            //  还剩下 [readerIdx += len -> writeIdx_)
            readerIdx_ += len;
        }
        else
        {
            //  len == readableBytes();
            retrieveAll();
        }
    }

    //  把onMessage函数上报的Buffer数据 转成string类型的数据返回
    std::string retrieveAllAsString()
    {
        return retrieveAsString(readableBytes());
    }

    std::string retrieveAsString(size_t len)
    {
        //  将缓冲区中可读的数据读出来
        std::string result(peek(), len);
        //  移动缓冲区指针 这叫复位？？
        retrieve(len);
        return result;
    }

    //  buffer_.size() - writeIdx_
    //  确保有len长度的可写空间
    void ensureWriteableBytes(size_t len)
    {
        if (writableBytes() < len)
        {
            makeSpace(len);
        }
    }

    //  向写缓冲区写入len长度的data
        //  [data,data+len] -> [writable ...)
    void append(const char *data,size_t len)
    {
        ensureWriteableBytes(len);
        std::copy(data,data+len,beginWrite());
        writerIdx_ += len;
    }

    //  返回可读数据的起始地址
    char *beginWrite()
    {
        return begin() + writerIdx_;
    }

    const char *beginWrite() const
    {
        return begin() + writerIdx_;
    }

    //  从fd中读数据
    ssize_t readFd(int fd,int *saveErrno);
    //  通过fd发送数据
    ssize_t writeFd(int fd,int *saveErrno);

private:
    char *begin()
    {
        return buffer_.begin().base();
    }

    const char *begin() const
    {
        return buffer_.begin().base();
    }

    //  readerIdx_和writerIdx_ 都回退到起点
    void retrieveAll()
    {
        readerIdx_ = writerIdx_ = kCheapPrepend;
    }

    //  扩容
        //  给谁扩容的？看样子是给写缓冲区扩容的
        //  那读缓冲区呢？？怎么办？覆盖吗？
        //  没看懂
    void makeSpace(size_t len)
    {
        //  ？不能通过移动来腾出空间
        if (writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            //  重新resize. 之前的内容还在
            buffer_.resize(writerIdx_ + len);
        }
        else
        {
            size_t readable = readableBytes();
            std::copy(begin() + readerIdx_,
                      begin() + writerIdx_,
                      begin() + kCheapPrepend);
            readerIdx_ = kCheapPrepend;
            writerIdx_ = readerIdx_ + readable;
        }
    }

private:
    std::vector<char> buffer_;
    size_t readerIdx_;
    size_t writerIdx_;
};

#endif