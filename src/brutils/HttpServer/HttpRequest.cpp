//
// Created by Burak Ongay on 10/11/2019.
//

#include "brutils/HttpServer/HttpRequest.h"

#include "brutils/json_parser.h"
#include "brutils/string_utils.h"

using namespace brutils;

HttpRequest::HttpRequest(br_object *parent) :
    br_object(parent),
    _method(UNKNOWN_METHOD),
    _version(UNKNOWN_VERSION)
{

}
HttpRequestMethod HttpRequest::method() const
{
  return _method;
}
HttpConnectionVersion HttpRequest::connectionVersion() const
{
  return _version;
}
std::string HttpRequest::path() const
{
  return _path;
}
std::string HttpRequest::query(const std::string &key) const
{
  return _query.at(toLower(key));
}
std::string HttpRequest::query(std::string &&key) const
{
  return _query.at(toLower(key));
}
std::map<std::string, std::string> HttpRequest::queryMap() const
{
  return _query;
}
std::vector<uint8_t> HttpRequest::rawBody() const
{
  return _rawBody;
}
std::string HttpRequest::header(const std::string &key) const
{
  return _headers.at(toLower(key));
}
std::string HttpRequest::header(std::string &&key) const
{
  return _headers.at(toLower(key));
}
std::map<std::string, std::string> HttpRequest::headerMap() const
{
  return _headers;
}
variant HttpRequest::bodyJson() const
{
  std::string_view body_view(reinterpret_cast<const char *>(_rawBody.data()), _rawBody.size());
  return json_parser::parse(body_view);
}
variant HttpRequest::bodyXml() const
{
  return brutils::variant();
}
void HttpServer_private::HttpRequest_SettersEnabled::setMethod(HttpRequestMethod method)
{
  _method = method;
}
void HttpServer_private::HttpRequest_SettersEnabled::setVersion(HttpConnectionVersion version)
{
  _version = version;
}
void HttpServer_private::HttpRequest_SettersEnabled::setPath(const std::string &path)
{
  _path = path;
}
void HttpServer_private::HttpRequest_SettersEnabled::setQuery(const std::map<std::string, std::string> &query)
{
  _query = query;
}
void HttpServer_private::HttpRequest_SettersEnabled::setRawBody(const std::vector<uint8_t> &rawBody)
{
  _rawBody = rawBody;
}
void HttpServer_private::HttpRequest_SettersEnabled::insertHeader(const std::string &key, const std::string &value)
{
  _headers.insert(std::make_pair(toLower(key), toLower(value)));
}
