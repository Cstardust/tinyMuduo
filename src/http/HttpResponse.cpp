#include "HttpResponse.h"
#include "Buffer.h"

#include <stdio.h>
#include <string.h>

void HttpResponse::appendToBuffer(Buffer* output) const
{
    // 状态行
    char buf[32] ={0};
    snprintf(buf, sizeof(buf), "HTTP/1.1 %d ", statusCode_);
    output->append(buf);
    output->append(statusMessage_);
    output->append("\r\n");

    //  首部行
    if (closeConnection_)
    {
        output->append("Connection: close\r\n");
    }
    else
    {
        snprintf(buf, sizeof(buf), "Content-Length: %zd\r\n", body_.size());
        output->append(buf);
        output->append("Connection: Keep-Alive\r\n");
    }

    for (const auto& header : headers_)
    {
        output->append(header.first);
        output->append(": ");
        output->append(header.second);
        output->append("\r\n");
    }
    output->append("\r\n");

    //  实体体
    output->append(body_); 
}