//
// Created by Burak Ongay on 10/11/2019.
//

#ifndef BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPREQUEST_H_
#define BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPREQUEST_H_

#include "brutils/br_object.h"

namespace brutils
{

enum HttpRequestMethod
{
  GET,
  HEAD,
  POST,
  PUT,
  PATCH,
  DELETE,
  CONNECT,
  OPTIONS,
  TRACE,
  UNKNOWN_METHOD
};

enum HttpConnectionVersion
{
  HTTP_10,
  HTTP_11,
  HTTP_20,
  HTTP_30,
  UNKNOWN_VERSION
};

class HttpRequest : public br_object
{
 public:
  HttpRequest(br_object *parent = nullptr);
  ~HttpRequest();

 protected:
  HttpRequestMethod _method;
  HttpConnectionVersion _version;
  std::string _path;
  std::map<std::string, std::string> _query;
  std::vector<uint8_t> _bodyContent; // TODO: implement different meaningful versions of body based on content-type
  std::map<std::string, std::string> _headers;

};

namespace HttpServer_private
{
class HttpRequest_SettersEnabled : public HttpRequest
{
 public:
  explicit HttpRequest_SettersEnabled(br_object *parent = nullptr) :
      HttpRequest(parent) {}
  ~HttpRequest_SettersEnabled() = default;

 public:
  void setMethod(HttpRequestMethod method)
  {
    _method = method;
  }
  void setVersion(HttpConnectionVersion version)
  {
    _version = version;
  }
  void setPath(const std::string &path)
  {
    _path = path;
  }
  void setQuery(const std::map<std::string, std::string> &query)
  {
    _query = query;
  }
  void setBodyContent(const std::vector<uint8_t> &bodyContent)
  {
    _bodyContent = bodyContent;
  }
  void insertHeader(const std::string &key, const std::string &value)
  {
    std::string myKey(key);
    std::transform(myKey.begin(), myKey.end(), myKey.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    _headers[myKey] = value;
    std::transform(_headers[myKey].begin(), _headers[myKey].end(), _headers[myKey].begin(),
                   [](unsigned char c){ return std::tolower(c); });
  }

};
}

}

#endif //BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPREQUEST_H_
