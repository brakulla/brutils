//
// Created by congay on 11/11/2019.
//

#include <brutils/json_generator.h>
#include "brutils/HttpServer/HttpResponse.h"

using namespace brutils;

HttpResponse::HttpResponse(br_object *parent) :
    br_object(parent),
    _statusMessageMap({
                          {HTTP_STATUS_CONTINUE, {'C', 'o', 'n', 't', 'i', 'n', 'u', 'e'}},
                          {HTTP_STATUS_SWITCHING_PROTOCOL, {'S', 'w', 'i', 't', 'c', 'h', 'i', 'n', 'g', ' ', 'P', 'r', 'o', 't', 'o', 'c', 'o', 'l'}},
                          {HTTP_STATUS_OK, {'O', 'K'}},
                          {HTTP_STATUS_CREATED, {'C', 'r', 'e', 'a', 't', 'e', 'd'}},
                          {HTTP_STATUS_ACCEPTED, {'A', 'c', 'c', 'e', 'p', 't', 'e', 'd'}},
                          {HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION, {'N', 'o', 'n', '-', 'A', 'u', 't', 'h', 'o', 'r', 'i', 't', 'a', 't', 'i', 'v', 'e', ' ', 'I', 'n', 'f', 'o', 'r', 'm', 'a', 't', 'i', 'o', 'n'}},
                          {HTTP_STATUS_NO_CONTENT, {'N', 'o', ' ', 'C', 'o', 'n', 't', 'e', 'n', 't'}},
                          {HTTP_STATUS_RESET_CONTENT, {'R', 'e', 's', 'e', 't', ' ', 'C', 'o', 'n', 't', 'e', 'n', 't'}},
                          {HTTP_STATUS_PARTIAL_CONTENT, {'P', 'a', 'r', 't', 'i', 'a', 'l', ' ', 'C', 'o', 'n', 't', 'e', 'n', 't'}},
                          {HTTP_STATUS_MULTIPLE_CHOICES, {'M', 'u', 'l', 't', 'i', 'p', 'l', 'e', ' ', 'C', 'h', 'o', 'i', 'c', 'e', 's'}},
                          {HTTP_STATUS_MOVED_PERMANENTLY, {'M', 'o', 'v', 'e', 'd', ' ', 'P', 'e', 'r', 'm', 'a', 'n', 'e', 'n', 't', 'l', 'y'}},
                          {HTTP_STATUS_FOUND, {'F', 'o', 'u', 'n', 'd'}},
                          {HTTP_STATUS_SEE_OTHER, {'S', 'e', 'e', ' ', 'O', 't', 'h', 'e', 'r'}},
                          {HTTP_STATUS_NOT_MODIFIED, {'N', 'o', 't', ' ', 'M', 'o', 'd', 'i', 'f', 'i', 'e', 'd'}},
                          {HTTP_STATUS_USE_PROXY, {'U', 's', 'e', ' ', 'P', 'r', 'o', 'x', 'y'}},
                          {HTTP_STATUS_TEMPORARY_REDIRECT, {'T', 'e', 'm', 'p', 'o', 'r', 'a', 'r', 'y', ' ', 'R', 'e', 'd', 'i', 'r', 'e', 'c', 't'}},
                          {HTTP_STATUS_PERMANENT_REDIRECT, {'P', 'e', 'r', 'm', 'a', 'n', 'e', 'n', 't', ' ', 'R', 'e', 'd', 'i', 'r', 'e', 'c', 't'}},
                          {HTTP_STATUS_BAD_REQUEST, {'B', 'a', 'd', ' ', 'R', 'e', 'q', 'u', 'e', 's', 't'}},
                          {HTTP_STATUS_UNAUTHORIZED, {'U', 'n', 'a', 'u', 't', 'h', 'o', 'r', 'i', 'z', 'e', 'd'}},
                          {HTTP_STATUS_PAYMENT_REQUIRED, {'P', 'a', 'y', 'm', 'e', 'n', 't', ' ', 'R', 'e', 'q', 'u', 'i', 'r', 'e', 'd'}},
                          {HTTP_STATUS_FORBIDDEN, {'F', 'o', 'r', 'b', 'i', 'd', 'd', 'e', 'n'}},
                          {HTTP_STATUS_NOT_FOUND, {'N', 'o', 't', ' ', 'F', 'o', 'u', 'n', 'd'}},
                          {HTTP_STATUS_METHOD_NOT_ALLOWED, {'M', 'e', 't', 'h', 'o', 'd', ' ', 'N', 'o', 't', ' ', 'A', 'l', 'l', 'o', 'w', 'e', 'd'}},
                          {HTTP_STATUS_NOT_ACCEPTABLE, {'N', 'o', 't', ' ', 'A', 'c', 'c', 'e', 'p', 't', 'a', 'b', 'l', 'e'}},
                          {HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED, {'P', 'r', 'o', 'x', 'y', ' ', 'A', 'u', 't', 'h', 'e', 'n', 't', 'i', 'c', 'a', 't', 'i','o', 'n', ',', 'R', 'e', 'q', 'u', 'i', 'r', 'e', 'd'}},
                          {HTTP_STATUS_REQUEST_TIMEOUT, {'R', 'e', 'q', 'u', 'e', 's', 't', ' ', 'T', 'i', 'm', 'e', '-', 'o', 'u', 't'}},
                          {HTTP_STATUS_CONFLICT, {'C', 'o', 'n', 'f', 'l', 'i', 'c', 't'}},
                          {HTTP_STATUS_GONE, {'G', 'o', 'n', 'e'}},
                          {HTTP_STATUS_LENGTH_REQUIRED, {'L', 'e', 'n', 'g', 't', 'h', ' ', 'R', 'e', 'q', 'u', 'i', 'r', 'e', 'd'}},
                          {HTTP_STATUS_PRECONDITION_FAILED, {'P', 'r', 'e', 'c', 'o', 'n', 'd', 'i', 't', 'i', 'o', 'n', ' ', 'F', 'a', 'i', 'l', 'e', 'd'}},
                          {HTTP_STATUS_PAYLOAD_TOO_LARGE, {'R', 'e', 'q', 'u', 'e', 's', 't', ' ', 'E', 'n', 't', 'i', 't', 'y', ' ', 'T', 'o', 'o', ' ', 'L', 'a', 'r', 'g', 'e'}},
                          {HTTP_STATUS_URI_TOO_LONG, {'R', 'e', 'q', 'u', 'e', 's', 't', '-', 'U', 'R', 'I', ' ', 'T', 'o', 'o', ' ', 'L', 'a', 'r', 'g', 'e'}},
                          {HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE, {'U', 'n', 's', 'u', 'p', 'p', 'o', 'r', 't', 'e', 'd', ' ', 'M', 'e', 'd', 'i', 'a', ' ', 'T', 'y', 'p', 'e'}},
                          {HTTP_STATUS_RANGE_NOT_SATISFIABLE, {'R', 'a', 'n', 'g', 'e', ' ', 'N', 'o', 't', ' ', 'S', 'a', 't', 'i', 's', 'f', 'i', 'a', 'b', 'l', 'e'}},
                          {HTTP_STATUS_EXPECTATION_FAILED, {'E', 'x', 'p', 'e', 'c', 't', 'a', 't', 'i', 'o', 'n', ' ', 'F', 'a', 'i', 'l', 'e', 'd'}},
                          {HTTP_STATUS_IM_A_TEAPOT, {'I', '\'', 'm', ' ', 'a', ' ', 'T', 'e', 'a', 'p', 'o', 't'}},
                          {HTTP_STATUS_UNPROCESSABLE_ENTITY, {'U', 'n', 'p', 'r', 'o', 'c', 'e', 's', 's', 'a', 'b', 'l', 'e', ' ', 'E', 'n', 't', 'i', 't', 'y'}},
                          {HTTP_STATUS_TOO_EARLY, {'T', 'o', 'o', ' ', 'E', 'a', 'r', 'l', 'y'}},
                          {HTTP_STATUS_UPGRADE_REQUIRED, {'U', 'p', 'g', 'r', 'a', 'd', 'e', ' ', 'R', 'e', 'q', 'u', 'i', 'r', 'e', 'd'}},
                          {HTTP_STATUS_PRECONDITION_REQUIRED, {'P', 'r', 'e', 'c', 'o', 'n', 'd', 'i', 't', 'i', 'o', 'n',' ', 'R', 'e', 'q', 'u', 'i', 'r', 'e', 'd'}},
                          {HTTP_STATUS_TOO_MANY_REQUESTS, {'T', 'o', 'o', ' ', 'M', 'a', 'n', 'y', ' ', 'R', 'e', 'q', 'u', 'e', 's', 't', 's'}},
                          {HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS, {'U', 'n', 'a', 'v', 'a', 'i', 'l', 'b', 'l', 'e', ' ', 'F', 'o', 'r', ' ', 'L', 'e', 'g', 'a', 'l', ' ', 'R', 'e', 'a', 's', 'o', 'n', 's'}},
                          {HTTP_STATUS_INTERNAL_SERVER_ERROR, {'I', 'n', 't', 'e', 'r', 'n', 'a', 'l', ' ', 'S', 'e', 'r', 'v', 'e', 'r', ' ', 'E', 'r', 'r', 'o', 'r'}},
                          {HTTP_STATUS_NOT_IMPLEMENTED, {'N', 'o', 't', ' ', 'I', 'm', 'p', 'l', 'e', 'm', 'e', 'n', 't', 'e', 'd'}},
                          {HTTP_STATUS_BAD_GATEWAY, {'B', 'a', 'd', ' ', 'G', 'a', 't', 'e', 'w', 'a', 'y'}},
                          {HTTP_STATUS_SERVICE_UNAVAILABLE, {'S', 'e', 'r', 'v', 'i', 'c', 'e', ' ', 'U', 'n', 'a', 'v', 'a', 'i', 'l', 'a', 'b', 'l', 'e'}},
                          {HTTP_STATUS_GATEWAY_TIMEOUT, {'G', 'e', 't', 'e', 'w', 'a', 'y', ' ', 'T', 'i', 'm', 'e', 'o', 'u', 't'}},
                          {HTTP_STATUS_VERSION_NOT_SUPPORTED, {'V', 'e', 'r', 's', 'i', 'o', 'n', ' ', 'N', 'o', 't', ' ', 'S', 'u', 'p', 'p', 'o', 'r', 't', 'e', 'd'}},
                          {HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED, {'N', 'e', 't', 'w', 'o', 'r', 'k', ' ' , 'A', 'u', 't', 'h', 'e', 'n', 't', 'i', 'c', 'a', 't', 'i', 'o', 'n', ' ', 'R', 'e', 'q', 'u', 'i', 'r', 'e', 'd'}}
                      }),
    _versionMap({
                    {HTTP_10, {'H', 'T', 'T', 'P', '/', '1', '.', '0'}},
                    {HTTP_11, {'H', 'T', 'T', 'P', '/', '1', '.', '1'}}
                }),
    _version(UNKNOWN_VERSION),
    _status(HTTP_STATUS_OK),
    _newLine({'\r', '\n'}),
    _headerSeparator({':', ' '}),
    _privateT(false)
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
  sendResponse({});
}
void HttpResponse::send(const std::string &content, const std::string &contentType)
{
  _headerMap["content-type"] = contentType;
  std::vector<std::byte> data(content.begin(), content.end());
  sendResponse(data);
}
void HttpResponse::send(std::string &&content, const std::string &contentType)
{
  _headerMap["content-type"] = contentType;
  std::vector<std::byte> data(content.begin(), content.end());
  sendResponse(data);
}
void HttpResponse::send(const std::string &content, std::string &&contentType)
{
  _headerMap["content-type"] = contentType;
  std::vector<std::byte> data(content.begin(), content.end());
  sendResponse(data);
}
void HttpResponse::send(std::string &&content, std::string &&contentType)
{
  _headerMap["content-type"] = contentType;
  std::vector<std::byte> data(content.begin(), content.end());
  sendResponse(data);
}
void HttpResponse::send(const std::vector<std::byte> &content, const std::string &contentType)
{
  _headerMap["content-type"] = contentType;
  sendResponse(content);
}
void HttpResponse::send(std::vector<std::byte> &&content, const std::string &contentType)
{
  _headerMap["content-type"] = contentType;
  sendResponse(content);
}
void HttpResponse::send(const std::vector<std::byte> &content, std::string &&contentType)
{
  _headerMap["content-type"] = contentType;
  sendResponse(content);
}
void HttpResponse::send(std::vector<std::byte> &&content, std::string &&contentType)
{
  _headerMap["content-type"] = contentType;
  sendResponse(content);
}
void HttpResponse::sendJson(const variant &map)
{
  auto json = json_generator::generate(map);
  std::vector<std::byte> jsonData(json.begin(), json.end());
  sendResponse(std::move(jsonData));
}
void HttpResponse::sendJson(variant &&map)
{
  auto json = json_generator::generate(map);
  std::vector<std::byte> jsonData(json.begin(), json.end());
  sendResponse(std::move(jsonData));
}
void HttpResponse::sendXml(const variant &map)
{
  // TODO: implement
}
void HttpResponse::sendXml(variant &&map)
{
  // TODO: implement
}
void HttpResponse::sendFile(const std::filesystem::path &filePath)
{
  // TODO: implement with a safe guard mechanism to prevent access to unauth folders
}
void HttpResponse::sendFile(std::filesystem::path &&filePath)
{
  // TODO: implement with a safe guard mechanism to prevent access to unauth folders
}
void HttpResponse::sendResponse(const std::vector<std::byte> &responseData)
{
  if (UNKNOWN_VERSION == _version) {
    _version = HTTP_11;
  }

  std::vector<std::byte> response;

  // response status line
  response.insert(response.end(), _versionMap.at(_version).begin(), _versionMap.at(_version).end());
  response.push_back(' ');
  auto status = convertHttpResponseStatusToVector(_status);
  response.insert(response.end(), status.begin(), status.end());
  response.insert(response.end(), _statusMessageMap.at(_status).begin(), _statusMessageMap.at(_status).end());
  response.insert(response.end(), _newLine.begin(), _newLine.end());

  // headers
  for (auto &headerPair: _headerMap) {
    response.insert(response.end(), headerPair.first.begin(), headerPair.first.end());
    response.insert(response.end(), _headerSeparator.begin(), _headerSeparator.end());
    response.insert(response.end(), headerPair.second.begin(), headerPair.second.end());
    response.insert(response.end(), _newLine.begin(), _newLine.end());
  }

  // body
  if (!responseData.empty()) {
    response.insert(response.end(), responseData.begin(), responseData.end());
  }
  if (_privateT) {
    ((HttpServer_private::HttpResponse_private*)this)->readyToWrite.emit(response);
  }
}
void HttpResponse::sendResponse(std::vector<std::byte> &&responseData)
{
  if (UNKNOWN_VERSION == _version) {
    _version = HTTP_11;
  }

  std::vector<std::byte> response;

  // response status line
  response.insert(response.end(), _versionMap.at(_version).begin(), _versionMap.at(_version).end());
  response.push_back(' ');
  auto status = convertHttpResponseStatusToVector(_status);
  response.insert(response.end(), status.begin(), status.end());
  response.insert(response.end(), _statusMessageMap.at(_status).begin(), _statusMessageMap.at(_status).end());
  response.insert(response.end(), _newLine.begin(), _newLine.end());

  // headers
  for (auto &headerPair: _headerMap) {
    response.insert(response.end(), headerPair.first.begin(), headerPair.first.end());
    response.insert(response.end(), _headerSeparator.begin(), _headerSeparator.end());
    response.insert(response.end(), headerPair.second.begin(), headerPair.second.end());
    response.insert(response.end(), _newLine.begin(), _newLine.end());
  }

  // body
  if (!responseData.empty()) {
    response.insert(response.end(), responseData.begin(), responseData.end());
  }
  if (_privateT) {
    ((HttpServer_private::HttpResponse_private*)this)->readyToWrite.emit(response);
  }
}
std::vector<std::byte> HttpResponse::convertHttpResponseStatusToVector(HttpResponseStatus status)
{
  std::string statusStr = std::to_string((int)status);
  std::vector<std::byte> statusVec(statusStr.begin(), statusStr.end());
  return statusVec;
}
