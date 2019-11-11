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
  explicit HttpRequest(br_object *parent = nullptr);
  ~HttpRequest() override = default;

 public:
  [[nodiscard]] HttpRequestMethod method() const;
  [[nodiscard]] HttpConnectionVersion connectionVersion() const;
  [[nodiscard]] std::string path() const;
  [[nodiscard]] std::string query(const std::string &key) const;
  [[nodiscard]] std::string query(std::string &&key) const;
  [[nodiscard]] std::map<std::string, std::string> queryMap() const;
  [[nodiscard]] std::vector<uint8_t> rawBody() const;
  [[nodiscard]] std::string header(const std::string &key) const;
  [[nodiscard]] std::string header(std::string &&key) const;
  [[nodiscard]] std::map<std::string, std::string> headerMap() const;

 protected:
  HttpRequestMethod _method;
  HttpConnectionVersion _version;
  std::string _path;
  std::map<std::string, std::string> _query;
  std::vector<uint8_t> _rawBody; // TODO: implement different meaningful versions of body based on content-type
  std::map<std::string, std::string> _headers;

  [[nodiscard]] std::string toLower(const std::string &str) const;

};

namespace HttpServer_private
{
class HttpRequest_SettersEnabled : public HttpRequest
{
 public:
  explicit HttpRequest_SettersEnabled(br_object *parent = nullptr) :
      HttpRequest(parent) {}
  ~HttpRequest_SettersEnabled() override = default;

 public:
  void setMethod(HttpRequestMethod method);
  void setVersion(HttpConnectionVersion version);
  void setPath(const std::string &path);
  void setQuery(const std::map<std::string, std::string> &query);
  void setRawBody(const std::vector<uint8_t> &rawBody);
  void insertHeader(const std::string &key, const std::string &value);
};
}

}

#endif //BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPREQUEST_H_
