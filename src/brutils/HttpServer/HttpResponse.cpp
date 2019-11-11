//
// Created by congay on 11/11/2019.
//

#include "brutils/HttpServer/HttpResponse.h"

using namespace brutils;

HttpResponse::HttpResponse(br_object *parent) :
    br_object(parent),
    readyToWrite(parent),
    _statusMessageMap({
                          {HTTP_STATUS_CONTINUE, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_SWITCHING_PROTOCOL, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_OK, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_CREATED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_ACCEPTED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_NO_CONTENT, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_RESET_CONTENT, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_PARTIAL_CONTENT, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_MULTIPLE_CHOICES, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_MOVED_PERMANENTLY, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_FOUND, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_SEE_OTHER, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_NOT_MODIFIED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_USE_PROXY, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_TEMPORARY_REDIRECT, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_BAD_REQUEST, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_UNAUTHORIZED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_FORBIDDEN, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_NOT_FOUND, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_METHOD_NOT_ALLOWED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_NOT_ACCEPTABLE, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_REQUEST_TIMEOUT, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_CONFLICT, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_GONE, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_LENGTH_REQUIRED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_PRECONDITION_FAILED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_PAYLOAD_TOO_LARGE, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_URI_TOO_LONG, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_RANGE_NOT_SATISFIABLE, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_EXPECTATION_FAILED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_UPGRADE_REQUIRED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_INTERNAL_SERVER_ERROR, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_NOT_IMPLEMENTED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_BAD_GATEWAY, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_SERVICE_UNAVAILABLE, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_GATEWAY_TIMEOUT, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_VERSION_NOT_SUPPORTED, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}}


    })
{

}
void HttpResponse::header(const std::string &key, const std::string &value)
{
  _headerMap.insert(std::make_pair(key, value));
}
void HttpResponse::header(std::string &&key, const std::string &value)
{
  _headerMap.insert(std::make_pair(key, value));
}
void HttpResponse::header(const std::string &key, std::string &&value)
{
  _headerMap.insert(std::make_pair(key, value));
}
void HttpResponse::header(std::string &&key, std::string &&value)
{
  _headerMap.insert(std::make_pair(key, value));
}
std::string HttpResponse::getHeader(const std::string &key)
{
  if (_headerMap.end() != _headerMap.find(key)) {
    return _headerMap.at(key);
  }
  return "";
}
std::string HttpResponse::getHeader(std::string &&key)
{
  if (_headerMap.end() != _headerMap.find(key)) {
    return _headerMap.at(key);
  }
  return "";
}
void HttpResponse::removeHeader(const std::string &key)
{
  _headerMap.erase(key);
}
void HttpResponse::removeHeader(std::string &&key)
{
  _headerMap.erase(key);
}
void HttpResponse::clearHeaders()
{
  _headerMap.clear();
}
void HttpResponse::status(const HttpResponseStatus &status)
{
  _status = status;
}
void HttpResponse::status(HttpResponseStatus &&status)
{
  _status = status;
}
void HttpResponse::status(const int &status)
{
  _status = (HttpResponseStatus)status;
}
void HttpResponse::status(int &&status)
{
  _status = (HttpResponseStatus)status;
}
void HttpResponse::send()
{
  std::vector<uint8_t> data;
  // TODO: emit readyToWrite signal
}
void HttpResponse::send(const std::shared_ptr<HttpContent> content)
{

}
void HttpResponse::send(const std::vector<uint8_t> &content, const std::string &contentType)
{

}
void HttpResponse::send(std::vector<uint8_t> &&content, const std::string &contentType)
{

}
void HttpResponse::send(const std::vector<uint8_t> &content, std::string &&contentType)
{

}
void HttpResponse::send(std::vector<uint8_t> &&content, std::string &&contentType)
{

}
void HttpResponse::sendJson(const variant_map &map)
{

}
void HttpResponse::sendJson(variant_map &&map)
{

}
void HttpResponse::sendJson(const std::string &jsonString)
{

}
void HttpResponse::sendJson(std::string &&jsonString)
{

}
void HttpResponse::sendXml(const variant_map &map)
{

}
void HttpResponse::sendXml(variant_map &&map)
{

}
void HttpResponse::sendXml(const std::string &xmlString)
{

}
void HttpResponse::sendXml(std::string &&xmlString)
{

}
void HttpResponse::sendFile(const std::filesystem::path &filePath)
{

}
void HttpResponse::sendFile(std::filesystem::path &&filePath)
{

}
void HttpResponse::sendResponse(std::vector<uint8_t> &responseData)
{
  std::vector<uint8_t> response;
  // TODO: insert http version string
  // TODO; insert status value as string
  // TODO: insert status string
  // TODO: insert headers
}
