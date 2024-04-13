/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 */

#ifndef HTTPPARSER_REQUEST_H
#define HTTPPARSER_REQUEST_H

#include <string>
#include <vector>
#include <sstream>

namespace httpparser
{

struct HttpRequest {
    HttpRequest()
        : versionMajor(0), versionMinor(0), keepAlive(false)
    {}
    
    struct HeaderItem
    {
        std::string name;
        std::string value;
    };

    std::string method;
    std::string uri;
    int versionMajor;
    int versionMinor;
    std::vector<HeaderItem> headers;    // 可以优化成unordered_map
    std::vector<char> content;
    bool keepAlive;

    std::string operator[](const std::string &field) const 
    {
        if(field == "content")
            return std::string(content.begin(), content.end());
        for(std::vector<HttpRequest::HeaderItem>::const_iterator it = headers.begin();
            it != headers.end(); ++it)
        {
            if (it->name == field) {
                return it->value;
            }   
        }
        return "NULL";
    }

    std::string inspect() const
    {
        std::stringstream stream;
        stream << method << " " << uri << " HTTP/"
               << versionMajor << "." << versionMinor << "\n";

        for(std::vector<HttpRequest::HeaderItem>::const_iterator it = headers.begin();
            it != headers.end(); ++it)
        {
            stream << it->name << ": " << it->value << "\n";
        }

        std::string data(content.begin(), content.end());
        stream << data << "\n";
        stream << "+ keep-alive: " << keepAlive << "\n";;
        return stream.str();
    }
};

} // namespace httpparser


#endif // HTTPPARSER_REQUEST_H
