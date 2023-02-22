#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include "noncopyable.h"
#include "Timestamp.h"
#include <unordered_map>
#include "Logger.h"

//  存储解析后的http request
class HttpRequest
{
public:
    //  支持的请求类型
    enum Method
    {
        kInvalid,
        kGet,
        kPost,
        kHead,
        kPut,
        kDelete
    };
    //  http 1.0 / http 1.1
    enum Version
    {
        kUnknown,
        kHttp10,
        kHttp11
    };

    HttpRequest()
        : method_(kInvalid),
          version_(kUnknown)
    {
    }

    Version version() const { return version_; }

    void setVersion(Version v) { version_ = v; }
    //  设置method of http request
    bool setMethod(const char *start, const char *end)
    {
        std::string m(start, end);
        if (m == "GET")
        {
            method_ = kGet;
        }
        else if (m == "POST")
        {
            method_ = kPost;
        }
        else if (m == "HEAD")
        {
            method_ = kHead;
        }
        else if (m == "PUT")
        {
            method_ = kPut;
        }
        else if (m == "DELETE")
        {
            method_ = kDelete;
        }
        else
        {
            method_ = kInvalid;
        }
        //  false if method is invalid
        return method_ != kInvalid;
    }

    Method method() const { return method_; }

    //  根据method 获取相应string方法类型
    const char *methodString() const
    {
        const char *result = "UNKNOWN";
        switch (method_)
        {
        case kGet:
            result = "GET";
            break;
        case kPost:
            result = "POST";
            break;
        case kHead:
            result = "HEAD";
            break;
        case kPut:
            result = "PUT";
            break;
        case kDelete:
            result = "DELETE";
            break;
        default:
            break;
        }
        return result;
    }

    //  set path part of url of request line
    void setPath(const char *start, const char *end) { path_.assign(start, end); }
    const std::string &path() const { return path_; }

    //  set entity body of the entity body
    void setQuery(const char *start, const char *end) { query_.assign(start, end); }
    const std::string &query() const { return query_; }

    void setReceiveTime(Timestamp t) { receiveTime_ = t; }
    Timestamp receiveTime() const { return receiveTime_; }

    //  根据首部行 , 读取其中的key-value到headers_中
    //  key : [start,colon)
    //  value [colon , end) 
    //  添加请求头键值对的字符串中，冒号前不能有空格 , 例如"key:value"或"key: value"是正确的，"key :value"就是错误的。
    void addHeader(const char *start, const char *colon, const char *end)
    {
        std::string field(start, colon);
        ++colon;

        //  去除 value [colon,end)的前部空格以及尾部空格
            //  skip space
        while (colon < end && isspace(*colon))
            ++colon;
        std::string value(colon, end);
            // value
        while (!value.empty() && isspace(value[value.size() - 1]))
            value.resize(value.size() - 1);
        
        LOG_DEBUG("kv %s <-> %s",field.c_str(),value.c_str());
        
        headers_[field] = value;
    }

    //  查找首部行中的k对应的v
    std::string getHeader(const std::string &field) const
    {
        std::string result;
        auto it = headers_.find(field);
        if (it != headers_.end())
            result = it->second;
        return result;
    }
    const std::unordered_map<std::string, std::string> &headers() const { return headers_; }

    void swap(HttpRequest &rhs)
    {
        std::swap(method_, rhs.method_);
        std::swap(version_, rhs.version_);
        path_.swap(rhs.path_);
        headers_.swap(rhs.headers_);
        query_.swap(rhs.query_);
        std::swap(receiveTime_, rhs.receiveTime_);
    }

private:
    Method method_;                                        // 请求行  -- http请求
    Version version_;                                      // 请求行  -- http版本
    std::string path_;                                     // 请求行  -- the path part of the url
    std::unordered_map<std::string, std::string> headers_; // 首部行(又叫请求头)
    std::string query_;                                    // 实体体
    Timestamp receiveTime_;                                // 请求时间 -- 请求时间
};

#endif // HTTP_HTTPREQUEST_H