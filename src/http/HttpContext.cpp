#include "HttpContext.h"
#include <algorithm>
#include "Buffer.h"

// GET /hello.txt HTTP/1.1
// User-Agent: curl/7.16.3 libcurl/7.16.3 OpenSSL/0.9.7l zlib/1.2.3
// Host: www.example.com
// Accept-Language: en, mi
// \r\n
// body ...

// 解析请求行[begin,end)
// 例如 GET /hello.txt HTTP/1.1
bool HttpContext::processRequestLine(const char *begin, const char *end)
{
    bool succeed = false;
    const char *start = begin;
    //  [start,end)找第一个空格
    const char *space = std::find(start, end, ' ');

    //  获取到method合法
    if (space != end && request_.setMethod(start, space))       //  如 GET
    {
        //  跳过空格
        start = space+1;                                        
        //  继续寻找下一个空格
        space = std::find(start, end, ' ');
        if (space != end)
        {
            // 查看是否有请求参数
            const char* question = std::find(start, space, '?');
            if (question != space)
            {
                //  请求行中的URL中可能带有请求参数，以问号"?"分割，后面的请求参数使用键值对方式，并作为实体体保存。
                //  设置访问路径
                request_.setPath(start, question);
                //  设置实体体
                request_.setQuery(question, space);
            }
            else
            {
                request_.setPath(start, space);                 //  如 /hello.txt 
            }
            start = space+1;
            //  获取http版本
            succeed = (end-start == 8 && std::equal(start, end-1, "HTTP/1."));
            if (succeed)
            {
                if (*(end-1) == '1')
                {
                    request_.setVersion(HttpRequest::kHttp11);
                }
                else if (*(end-1) == '0')
                {
                    request_.setVersion(HttpRequest::kHttp10);
                }
                else
                {
                    succeed = false;
                }
            }
        }
    }  
    return succeed;
}

// return false if any error
bool HttpContext::parseRequest(Buffer* buf, Timestamp receiveTime)
{
    bool ok = false;
    bool hasMore = true;
    while (hasMore)
    {
        // 状态机 
        if (state_ == kExpectRequestLine)
        {
            // 找到 \r\n 位置
            const char* crlf = buf->findCRLF();
            if (crlf)
            {
                // 从可读区读取请求行 , [peek(), crlf + 2) 是一行
                ok = processRequestLine(buf->peek(), crlf);
                if (ok)
                {
                    request_.setReceiveTime(receiveTime);
                    // readerIndex 向后移动位置直到 crlf + 2
                    buf->retrieveUntil(crlf + 2);
                    // 状态转移，接下来解析请求头
                    state_ = kExpectHeaders;
                }
                //  解析请求头失败. return false
                else
                {
                    hasMore = false;
                }
            }
            //  没找到 解析请求头失败 return false
            else
            {
                hasMore = false;
            }
        }
        // 解析首部行
        else if (state_ == kExpectHeaders)
        {
            //  一行首部 \r\n
            const char* crlf = buf->findCRLF();
            if (crlf)
            {
                // colon -> : 
                const char* colon = std::find(buf->peek(), crlf, ':');
                if (colon != crlf)
                {
                    // 添加状态首部
                    LOG_DEBUG("add header!");
                    request_.addHeader(buf->peek(), colon, crlf);
                }
                else // 没有 : . 解析完毕, break
                {
                    // empty line, end of header
                    state_ = kGotAll;
                    hasMore = false;
                }
                buf->retrieveUntil(crlf + 2);
            }
            else
            {
                // empty line, end of header
                state_ = kGotAll;
                hasMore = false;
            }
        }
        // 解析请求体，可以看到这里没有做出处理，只支持GET请求
        else if (state_ == kExpectBody)
        {
            LOG_INFO("server only support GET method now");
        }
    }
    return ok;
}