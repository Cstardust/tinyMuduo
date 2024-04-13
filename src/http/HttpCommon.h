#ifndef COMMON_H
#define COMMON_H

#include "HttpRequest.h"
#include "HttpResponse.h"


std::ostream & operator<< (std::ostream &stream, const httpparser::HttpRequest &req)
{
    stream << req.method << " " << req.uri << " HTTP/" 
           << req.versionMajor << "." << req.versionMinor << "\n";
           
    for(std::vector<httpparser::HttpRequest::HeaderItem>::const_iterator it = req.headers.begin();
        it != req.headers.end(); ++it)
    {
        stream << it->name << ": " << it->value << "\n";
    }
    
    stream << req.content.data() << "\n";
    stream << "+ keep-alive: " << req.keepAlive << "\n";;
    
    return stream;
}

std::ostream & operator<< (std::ostream &stream, const httpparser::HttpResponse &resp)
{
    stream << "HTTP/" << resp.versionMajor << "." << resp.versionMinor
           << " " << resp.statusCode << " " << resp.status << "\n";

    for(std::vector<httpparser::HttpResponse::HeaderItem>::const_iterator it = resp.headers.begin();
        it != resp.headers.end(); ++it)
    {
        stream << it->name << ": " << it->value << "\n";
    }

    stream << resp.content.data() << "\n";

    return stream;
}

#endif // COMMON_H
