#ifndef _HTTP_CONTEXT_H_
#define _HTTP_CONTEXT_H_

#include "HttpRequest.h"

class Buffer;

//  FSM parsing http request
class HttpContext
{
public:
    // HTTP请求状态
    enum HttpRequestParseState
    {
        kExpectRequestLine, // parsing requestline(请求行)
        kExpectHeaders,     // parsing headerline(首部行)
        kExpectBody,        // parsing entity body(实体体)
        kGotAll,            // finished
    };

    //  默认解析请求行
    HttpContext()
        : state_(kExpectRequestLine)
    {
    }

    bool parseRequest(Buffer *buf, Timestamp receiveTime);

    bool gotAll() const { return state_ == kGotAll; }

    // 重置HttpContext状态
    void reset()
    {
        state_ = kExpectRequestLine;
        HttpRequest dummy;
        request_.swap(dummy);
    }

    const HttpRequest &request() const { return request_; }
    HttpRequest &request() { return request_; }

private:
    bool processRequestLine(const char *begin, const char *end);

    HttpRequestParseState state_;
    HttpRequest request_;
};

#endif // HTTP_HTTPCONTEXT_H