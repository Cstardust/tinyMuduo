#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include "noncopyable.h"
#include "Timestamp.h"
#include "Buffer.h"
#include <unordered_map>

class HttpResponse
{
public:
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

  explicit HttpResponse(bool close) : statusCode_(kUnknown), closeConnection_(close) {}

  void setStatusCode(HttpStatusCode code) { statusCode_ = code; }

  void setStatusMessage(const string &message) { statusMessage_ = message; }

  void setCloseConnection(bool on) { closeConnection_ = on; }

  bool closeConnection() const { return closeConnection_; }

  void setContentType(const string &contentType) { addHeader("Content-Type", contentType); }

  void addHeader(const string &key, const string &value) { headers_[key] = value; }

  void setBody(const string &body) { body_ = body; }

  //  将HttpResponse序列化到Buffer中
  void appendToBuffer(Buffer *output) const;  

private:
  HttpStatusCode statusCode_;                  // 状态行 statusCode
  string statusMessage_;                       // 状态行 statusCode文字
  std::unordered_map<string, string> headers_; // 首部行  
  string body_;                                // 实体体
  bool closeConnection_;                       // 是否关闭连接
};

#endif