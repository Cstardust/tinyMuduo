/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 */

#ifndef HTTPPARSER_RESPONSE_H
#define HTTPPARSER_RESPONSE_H

#include <string>
#include <vector>
#include <sstream>

namespace httpparser
{

struct HttpResponse {
    HttpResponse(int vmax = 0, int vmin = 0, bool ka = false, int code = 0)
        : versionMajor(vmax), versionMinor(vmin), keepAlive(ka), statusCode(code)
    {}
    
    struct HeaderItem
    {
        std::string name;
        std::string value;
        HeaderItem(const std::string &k = "", const std::string &v = ""):
            name(k), value(v) {}
    };

    //  status code
    enum HttpStatusCode
    {
        kUnknown,
        k200Ok = 200,               // ok
        k301MovedPermanently = 301, // 资源被永久转移
        k400BadRequest = 400,       // http request错误
        k404NotFound = 404,         // not found
        k505NotVersionSupported = 505   //  server不支持http request的http版本
    };

    int versionMajor;
    int versionMinor;
    std::vector<HeaderItem> headers;
    std::vector<char> content;
    bool keepAlive;
    
    unsigned int statusCode;
    std::string status;

    void setContent(const std::string &str) {
        content.assign(str.begin(), str.end());
    }

    void setStatusCode(HttpStatusCode code) {
        statusCode = code;
    }

    void setStatusMsg(const std::string &str) {
        status = str;
    }

    void addHeader(const std::string k, const std::string v) {
        headers.emplace_back(k, v);
    }

    std::string inspect() const
    {
        std::stringstream stream;
        stream << "HTTP/" << versionMajor << "." << versionMinor
               << " " << statusCode << " " << status << "\r\n";

        for(std::vector<HttpResponse::HeaderItem>::const_iterator it = headers.begin();
            it != headers.end(); ++it)
        {
            stream << it->name << ": " << it->value << "\r\n";
        }
        stream << "\r\n";
        std::string data(content.begin(), content.end());
        // stream << data << "\r\n";
        stream << data;
        return stream.str();
    }
};

} // namespace httpparser

#endif // HTTPPARSER_RESPONSE_H

