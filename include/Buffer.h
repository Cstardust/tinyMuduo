#ifndef MY_MUDUO_BUFFER
#define MY_MUDUO_BUFFER

#include <vector>
#include <string>
#include <algorithm>
#include<errno.h>
#include "Logger.h"

/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size

//  网络库底层缓冲区
//  Buffer还没实践
//  Buffer中的readable writeable都是针对Buffer来说的。：可从buffer中读出(读出来给fd) ；可向buffer中写入（从fd中读出来写给buffer）
//  [readerIdx,writerIdx) 有效数据 可从buffer中读出
//  [writerIdx,size)      剩余的可向buffer中写入的byte数量
//  Interface for user
    //  readFd fd               : 用户使用readFd   将fd输入给用户程序的数据 输入给Bufferd的writable区域[writeIdx,size)
    //  writeFd  fd             : 用户使用writeFd  将Buffer中的readable数据 全部输入给fd文件
    //  retrieve                : 用户从buffer中拿走readable bytes。以string形式获得。会改变readIdx
    //  retrieveAllAsString     : 用户从buffer中拿走全部readable bytes。以string形式获得。会改变readIdx
    //  user不必关心Buffer的大小。Buffer是自适应扩张的。
        //  唯一扩张的原因就是调用Buffer的append（向缓冲区中写数据）。一般是user通过readFd调用。
        //  Buffer会自己扩充内存 管理writeIdx readIdx
    //  因此 user只需要负责 读数据到buffer，从buffer中拿出数据。
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
        LOG_INFO("readerIdx = %ld ; writerIdx = %ld; total size = %lu\n",readerIdx_,writerIdx_,buffer_.size());
    }


    //  都是stack上的 没必要重载析构
    ~Buffer()
    {
        LOG_INFO("readerIdx = %ld ; writerIdx = %ld; total size = %lu\n",readerIdx_,writerIdx_,buffer_.size());
    }

    //  返回可读的字节数
    size_t readableBytes() const
    {
        return writerIdx_ - readerIdx_;
    }

    //  返回可以写入Buffer的字节数
    size_t writableBytes() const
    {
        return buffer_.size() - writerIdx_;
    }

    //  返回prependable 的字节数
    size_t prependableBytes() const
    {
        return readerIdx_;
    }

    //  把onMessage函数上报的Buffer数据 转成string类型的数据返回
    std::string retrieveAllAsString()
    {
        return retrieveAsString(readableBytes());
    }

    //  从readerIdx开始 读取len bytes char
    //  [readerIdx,readerIdx+len)
    std::string retrieveAsString(size_t len)
    {
        //  [readerIdx,readerIdx+len) 转化成string
        std::string result(peek(), len);
        //  移动readerIdx 至还没读的char
        retrieve(len);
        return result;
    }


    //  返回readerIdx指向的内存地址
    //  [readerIdx,writerIdx)中的readerIdx指向的char的地址
    const char *peek() const
    {
        //  vector 底层是连续的
        return begin() + readerIdx_;
    }

    //  读走len长度的字节之后 移动readIdx_ ?
    //  on Message : string <- Buffer
    void retrieve(size_t len)
    {
        //  应用只读取了可读缓冲区数据的一部分，就是len
        //  还剩下 [readerIdx += len -> writeIdx_)
        if (len < readableBytes())
        {

            readerIdx_ += len;
        }
        //  readerIdx开始的有效数据全部被读走
        //  复位即可
        else
        {
            //  len == readableBytes();(>readableBytes()也没用，最多就读readableBytes())
            retrieveAll();
        }
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
        //  data -> [writable ...)
    void append(const char *data,size_t len)
    {
        //  确保写空间足够
        ensureWriteableBytes(len);
        //  从extrabuf中写入buffer
        std::copy(data,data+len,beginWrite());
        //  移动writeIdx 一定能够移动len长度。因为write到buffer的writeable之前已经ensure
        writerIdx_ += len;
       LOG_INFO("after read all into buffer : len = %ld ; readerIdx = %ld ; writerIdx = %ld ; total size = %lu\n",len,readerIdx_,writerIdx_,buffer_.size());
    }

    //  返回writeIdx指向的内存地址
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
        //  扩容原因：buffer的可写空间[writerIdx,size())不够大
        //  扩容目的（效果）：
            //  确保buffer的可写空间 也即 [writerIdx,size()) >= len; 
            //  且不破坏buffer中的可读数据 即 [readerIdx,writerIdx)
            //  writerIdx readerIdx的相关移动都在这个函数里处理了。
    void makeSpace(size_t len)
    {
        //  buffer中剩余的可写空间总大小 writable size：writeableBytes + prependableBytes - kCheapPrepend
        //  要写入的数据大小 data size：len
        //  writable size < len
            //  只能扩展vector 扩展lenbytes
        //  writable size >= len   
            //  移动readable bytes : 即[readerIdx,writeIdx)到readerIdx一开始的起点，即buffer[kCheapPrepend]地址
            //  这样就将prependableBytes - kCheapPrepend 和 writableBytes拼接在了一起
            //  [readerIdx,writerIdx) [writerIdx, old writeableBytes + prependableBytes - kCheapPrepend)
        if (writableBytes() + prependableBytes() < len + kCheapPrepend)
        {
            LOG_INFO("before resize : readerIdx = %ld ; writerIdx = %ld; total size = %lu\n",readerIdx_,writerIdx_,buffer_.size());
            //  resize 重新分配一块新内存. 不必移动readerIdx，writerIdx
            buffer_.resize(writerIdx_ + len);
            LOG_INFO("after resize : readerIdx = %ld ; writerIdx = %ld; total size = %lu\n",readerIdx_,writerIdx_,buffer_.size());
        }
        else
        {
            //  移动readable bytes。需要移动readerIdx、writerIdx
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