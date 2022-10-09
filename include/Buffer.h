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
//  Buffer中的readable writeable都是针对Buffer来说的。：可从buffer中读出(读出来给fd) ；可向buffer中写入（从fd中读出来写给buffer）
//  [readerIdx,writerIdx) 有效数据 可从buffer中读出
//  [writerIdx,size)      剩余的可向buffer中写入的byte数量
//  Interface for user
    //  readFd fd               : 用户使用readFd   从fd读取数据 输入给Bufferd的writable区域[writeIdx,size)
    //  writeFd  fd             : 用户使用writeFd  将Buffer中的readable数据 全部输入给fd文件
    //  retrieveAllAsString     : 用户从buffer中拿走全部readable bytes。以string形式获得。会改变readIdx
    //  append                  : Buffer使用者 通过append 向Buffer中增添数据。buffer数据增加有两种方式：一个是readFd 一个是append 
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
    {}

    //  都是stack上的 没必要重载析构
    ~Buffer(){}
    //  从fd中读数据
    ssize_t readFd(int fd,int *saveErrno);
    //  通过fd发送数据
    ssize_t writeFd(int fd,int *saveErrno);

    //  buffer中的所有可读数据 转成string类型的数据返回
        //  一般会被user编写的的onMessage调用 
    std::string retrieveAllAsString()
    {
        return retrieveAsString(readableBytes());
    }

    //  从readerIdx开始 读取len bytes char
    std::string retrieveAsString(size_t len)
    {
        //  [readerIdx,readerIdx+len) 转化成string
        std::string result(peek(), len);
        //  移动readerIdx
        retrieve(len);
        return result;
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

private:

    //  作用：清除Buffer里已经使用完的字节
        //  buffer的使用者不必自己负责清除已经用过的字节 ，即不必自己调用retrieve(len)
        //  retrieve已经封装在writeFd 和 retrieveAsString(len) 以及 retrieveAllAsString()了 
        //  从buffer中取走len长度的字节之后(无论是读走还是写出) 移动readIdx_ 
        //  readerIdx的移动被封装在retrieve里
        //  writerIdx的移动没有封装
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
            //  len >= readableBytes(); 
            retrieveAll();
        }
    }

    //  确保有len长度的可写空间
    void ensureWriteableBytes(size_t len)
    {
        if (writableBytes() < len)
        {
            makeSpace(len);
        }
    }

    //  [readerIdx,writerIdx) return readerIdx指向的char的地址
    const char *peek() const
    {
        //  vector 底层是连续的
        return begin() + readerIdx_;
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
            //  resize 重新分配一块新内存. 不必移动readerIdx，writerIdx
            buffer_.resize(writerIdx_ + len);
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