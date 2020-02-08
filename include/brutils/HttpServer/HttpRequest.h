/**
 * @file HttpRequest.h
 *
 * Description
 */

#ifndef BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPREQUEST_H_
#define BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPREQUEST_H_

#include "brutils/br_object.h"
#include "brutils/variant.h"

namespace brutils
{

enum HttpMethod
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

enum class HttpVersion
{
  HTTP_10,
  HTTP_11,
  HTTP_20,
  HTTP_30,
  UNKNOWN
};

class HttpRequest : public br_object
{
 public:
  explicit HttpRequest(br_object *parent = nullptr);
  ~HttpRequest() override = default;

 public:
  [[nodiscard]] HttpMethod method() const;
  [[nodiscard]] HttpVersion connectionVersion() const;
  [[nodiscard]] std::string path() const;
  [[nodiscard]] std::string query(const std::string &key) const;
  [[nodiscard]] std::string query(std::string &&key) const;
  [[nodiscard]] std::map<std::string, std::string> queryMap() const;
  [[nodiscard]] std::vector<std::byte> rawBody() const;
  [[nodiscard]] std::string header(const std::string &key) const;
  [[nodiscard]] std::string header(std::string &&key) const;
  [[nodiscard]] std::map<std::string, std::string> headerMap() const;
  [[nodiscard]] variant bodyJson() const;
  [[nodiscard]] variant bodyXml() const;

 protected:
  HttpMethod _method;
  HttpVersion _version;
  std::string _path;
  std::map<std::string, std::string> _query;
  std::vector<std::byte> _rawBody; // TODO: implement different meaningful versions of body based on content-type
  std::map<std::string, std::string> _headers;

};

namespace HttpServer_private
{
class HttpRequest_private : public HttpRequest
{
 public:
  explicit HttpRequest_private(br_object *parent = nullptr) :
      HttpRequest(parent) {}
  ~HttpRequest_private() override = default;

 public:
  void setMethod(HttpMethod method);
  void setVersion(HttpVersion version);
  void setPath(const std::string &path);
  void setQuery(const std::map<std::string, std::string> &query);
  void setRawBody(const std::vector<std::byte> &rawBody);
  void insertHeader(const std::string &key, const std::string &value);
};
}

}

#endif //BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPREQUEST_H_
