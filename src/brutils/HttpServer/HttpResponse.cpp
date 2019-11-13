//
// Created by congay on 11/11/2019.
//

#include <brutils/json_generator.h>
#include "brutils/HttpServer/HttpResponse.h"

using namespace brutils;

HttpResponse::HttpResponse(br_object *parent) :
    br_object(parent),
    _statusMessageMap({
                          {HTTP_STATUS_CONTINUE, {std::byte{'C'}, std::byte{'o'}, std::byte{'n'}, std::byte{'t'}, std::byte{'i'}, std::byte{'n'}, std::byte{'u'}, std::byte{'e'}}},
                          {HTTP_STATUS_SWITCHING_PROTOCOL, {std::byte{'S'}, std::byte{'w'}, std::byte{'i'}, std::byte{'t'}, std::byte{'c'}, std::byte{'h'}, std::byte{'i'}, std::byte{'n'}, std::byte{'g'}, std::byte{' '}, std::byte{'P'}, std::byte{'r'}, std::byte{'o'}, std::byte{'t'}, std::byte{'o'}, std::byte{'c'}, std::byte{'o'}, std::byte{'l'}}},
                          {HTTP_STATUS_OK, {std::byte{'O'}, std::byte{'K'}}},
                          {HTTP_STATUS_CREATED, {std::byte{'C'}, std::byte{'r'}, std::byte{'e'}, std::byte{'a'}, std::byte{'t'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_ACCEPTED, {std::byte{'A'}, std::byte{'c'}, std::byte{'c'}, std::byte{'e'}, std::byte{'p'}, std::byte{'t'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION, {std::byte{'N'}, std::byte{'o'}, std::byte{'n'}, std::byte{'-'}, std::byte{'A'}, std::byte{'u'}, std::byte{'t'}, std::byte{'h'}, std::byte{'o'}, std::byte{'r'}, std::byte{'i'}, std::byte{'t'}, std::byte{'a'}, std::byte{'t'}, std::byte{'i'}, std::byte{'v'}, std::byte{'e'}, std::byte{' '}, std::byte{'I'}, std::byte{'n'}, std::byte{'f'}, std::byte{'o'}, std::byte{'r'}, std::byte{'m'}, std::byte{'a'}, std::byte{'t'}, std::byte{'i'}, std::byte{'o'}, std::byte{'n'}}},
                          {HTTP_STATUS_NO_CONTENT, {std::byte{'N'}, std::byte{'o'}, std::byte{' '}, std::byte{'C'}, std::byte{'o'}, std::byte{'n'}, std::byte{'t'}, std::byte{'e'}, std::byte{'n'}, std::byte{'t'}}},
                          {HTTP_STATUS_RESET_CONTENT, {std::byte{'R'}, std::byte{'e'}, std::byte{'s'}, std::byte{'e'}, std::byte{'t'}, std::byte{' '}, std::byte{'C'}, std::byte{'o'}, std::byte{'n'}, std::byte{'t'}, std::byte{'e'}, std::byte{'n'}, std::byte{'t'}}},
                          {HTTP_STATUS_PARTIAL_CONTENT, {std::byte{'P'}, std::byte{'a'}, std::byte{'r'}, std::byte{'t'}, std::byte{'i'}, std::byte{'a'}, std::byte{'l'}, std::byte{' '}, std::byte{'C'}, std::byte{'o'}, std::byte{'n'}, std::byte{'t'}, std::byte{'e'}, std::byte{'n'}, std::byte{'t'}}},
                          {HTTP_STATUS_MULTIPLE_CHOICES, {std::byte{'M'}, std::byte{'u'}, std::byte{'l'}, std::byte{'t'}, std::byte{'i'}, std::byte{'p'}, std::byte{'l'}, std::byte{'e'}, std::byte{' '}, std::byte{'C'}, std::byte{'h'}, std::byte{'o'}, std::byte{'i'}, std::byte{'c'}, std::byte{'e'}, std::byte{'s'}}},
                          {HTTP_STATUS_MOVED_PERMANENTLY, {std::byte{'M'}, std::byte{'o'}, std::byte{'v'}, std::byte{'e'}, std::byte{'d'}, std::byte{' '}, std::byte{'P'}, std::byte{'e'}, std::byte{'r'}, std::byte{'m'}, std::byte{'a'}, std::byte{'n'}, std::byte{'e'}, std::byte{'n'}, std::byte{'t'}, std::byte{'l'}, std::byte{'y'}}},
                          {HTTP_STATUS_FOUND, {std::byte{'F'}, std::byte{'o'}, std::byte{'u'}, std::byte{'n'}, std::byte{'d'}}},
                          {HTTP_STATUS_SEE_OTHER, {std::byte{'S'}, std::byte{'e'}, std::byte{'e'}, std::byte{' '}, std::byte{'O'}, std::byte{'t'}, std::byte{'h'}, std::byte{'e'}, std::byte{'r'}}},
                          {HTTP_STATUS_NOT_MODIFIED, {std::byte{'N'}, std::byte{'o'}, std::byte{'t'}, std::byte{' '}, std::byte{'M'}, std::byte{'o'}, std::byte{'d'}, std::byte{'i'}, std::byte{'f'}, std::byte{'i'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_USE_PROXY, {std::byte{'U'}, std::byte{'s'}, std::byte{'e'}, std::byte{' '}, std::byte{'P'}, std::byte{'r'}, std::byte{'o'}, std::byte{'x'}, std::byte{'y'}}},
                          {HTTP_STATUS_TEMPORARY_REDIRECT, {std::byte{'T'}, std::byte{'e'}, std::byte{'m'}, std::byte{'p'}, std::byte{'o'}, std::byte{'r'}, std::byte{'a'}, std::byte{'r'}, std::byte{'y'}, std::byte{' '}, std::byte{'R'}, std::byte{'e'}, std::byte{'d'}, std::byte{'i'}, std::byte{'r'}, std::byte{'e'}, std::byte{'c'}, std::byte{'t'}}},
                          {HTTP_STATUS_PERMANENT_REDIRECT, {std::byte{'P'}, std::byte{'e'}, std::byte{'r'}, std::byte{'m'}, std::byte{'a'}, std::byte{'n'}, std::byte{'e'}, std::byte{'n'}, std::byte{'t'}, std::byte{' '}, std::byte{'R'}, std::byte{'e'}, std::byte{'d'}, std::byte{'i'}, std::byte{'r'}, std::byte{'e'}, std::byte{'c'}, std::byte{'t'}}},
                          {HTTP_STATUS_BAD_REQUEST, {std::byte{'B'}, std::byte{'a'}, std::byte{'d'}, std::byte{' '}, std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'e'}, std::byte{'s'}, std::byte{'t'}}},
                          {HTTP_STATUS_UNAUTHORIZED, {std::byte{'U'}, std::byte{'n'}, std::byte{'a'}, std::byte{'u'}, std::byte{'t'}, std::byte{'h'}, std::byte{'o'}, std::byte{'r'}, std::byte{'i'}, std::byte{'z'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_PAYMENT_REQUIRED, {std::byte{'P'}, std::byte{'a'}, std::byte{'y'}, std::byte{'m'}, std::byte{'e'}, std::byte{'n'}, std::byte{'t'}, std::byte{' '}, std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'i'}, std::byte{'r'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_FORBIDDEN, {std::byte{'F'}, std::byte{'o'}, std::byte{'r'}, std::byte{'b'}, std::byte{'i'}, std::byte{'d'}, std::byte{'d'}, std::byte{'e'}, std::byte{'n'}}},
                          {HTTP_STATUS_NOT_FOUND, {std::byte{'N'}, std::byte{'o'}, std::byte{'t'}, std::byte{' '}, std::byte{'F'}, std::byte{'o'}, std::byte{'u'}, std::byte{'n'}, std::byte{'d'}}},
                          {HTTP_STATUS_METHOD_NOT_ALLOWED, {std::byte{'M'}, std::byte{'e'}, std::byte{'t'}, std::byte{'h'}, std::byte{'o'}, std::byte{'d'}, std::byte{' '}, std::byte{'N'}, std::byte{'o'}, std::byte{'t'}, std::byte{' '}, std::byte{'A'}, std::byte{'l'}, std::byte{'l'}, std::byte{'o'}, std::byte{'w'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_NOT_ACCEPTABLE, {std::byte{'N'}, std::byte{'o'}, std::byte{'t'}, std::byte{' '}, std::byte{'A'}, std::byte{'c'}, std::byte{'c'}, std::byte{'e'}, std::byte{'p'}, std::byte{'t'}, std::byte{'a'}, std::byte{'b'}, std::byte{'l'}, std::byte{'e'}}},
                          {HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED, {std::byte{'P'}, std::byte{'r'}, std::byte{'o'}, std::byte{'x'}, std::byte{'y'}, std::byte{' '}, std::byte{'A'}, std::byte{'u'}, std::byte{'t'}, std::byte{'h'}, std::byte{'e'}, std::byte{'n'}, std::byte{'t'}, std::byte{'i'}, std::byte{'c'}, std::byte{'a'}, std::byte{'t'}, std::byte{'i'},std::byte{'o'}, std::byte{'n'}, std::byte{','}, std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'i'}, std::byte{'r'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_REQUEST_TIMEOUT, {std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'e'}, std::byte{'s'}, std::byte{'t'}, std::byte{' '}, std::byte{'T'}, std::byte{'i'}, std::byte{'m'}, std::byte{'e'}, std::byte{'-'}, std::byte{'o'}, std::byte{'u'}, std::byte{'t'}}},
                          {HTTP_STATUS_CONFLICT, {std::byte{'C'}, std::byte{'o'}, std::byte{'n'}, std::byte{'f'}, std::byte{'l'}, std::byte{'i'}, std::byte{'c'}, std::byte{'t'}}},
                          {HTTP_STATUS_GONE, {std::byte{'G'}, std::byte{'o'}, std::byte{'n'}, std::byte{'e'}}},
                          {HTTP_STATUS_LENGTH_REQUIRED, {std::byte{'L'}, std::byte{'e'}, std::byte{'n'}, std::byte{'g'}, std::byte{'t'}, std::byte{'h'}, std::byte{' '}, std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'i'}, std::byte{'r'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_PRECONDITION_FAILED, {std::byte{'P'}, std::byte{'r'}, std::byte{'e'}, std::byte{'c'}, std::byte{'o'}, std::byte{'n'}, std::byte{'d'}, std::byte{'i'}, std::byte{'t'}, std::byte{'i'}, std::byte{'o'}, std::byte{'n'}, std::byte{' '}, std::byte{'F'}, std::byte{'a'}, std::byte{'i'}, std::byte{'l'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_PAYLOAD_TOO_LARGE, {std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'e'}, std::byte{'s'}, std::byte{'t'}, std::byte{' '}, std::byte{'E'}, std::byte{'n'}, std::byte{'t'}, std::byte{'i'}, std::byte{'t'}, std::byte{'y'}, std::byte{' '}, std::byte{'T'}, std::byte{'o'}, std::byte{'o'}, std::byte{' '}, std::byte{'L'}, std::byte{'a'}, std::byte{'r'}, std::byte{'g'}, std::byte{'e'}}},
                          {HTTP_STATUS_URI_TOO_LONG, {std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'e'}, std::byte{'s'}, std::byte{'t'}, std::byte{'-'}, std::byte{'U'}, std::byte{'R'}, std::byte{'I'}, std::byte{' '}, std::byte{'T'}, std::byte{'o'}, std::byte{'o'}, std::byte{' '}, std::byte{'L'}, std::byte{'a'}, std::byte{'r'}, std::byte{'g'}, std::byte{'e'}}},
                          {HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE, {std::byte{'U'}, std::byte{'n'}, std::byte{'s'}, std::byte{'u'}, std::byte{'p'}, std::byte{'p'}, std::byte{'o'}, std::byte{'r'}, std::byte{'t'}, std::byte{'e'}, std::byte{'d'}, std::byte{' '}, std::byte{'M'}, std::byte{'e'}, std::byte{'d'}, std::byte{'i'}, std::byte{'a'}, std::byte{' '}, std::byte{'T'}, std::byte{'y'}, std::byte{'p'}, std::byte{'e'}}},
                          {HTTP_STATUS_RANGE_NOT_SATISFIABLE, {std::byte{'R'}, std::byte{'a'}, std::byte{'n'}, std::byte{'g'}, std::byte{'e'}, std::byte{' '}, std::byte{'N'}, std::byte{'o'}, std::byte{'t'}, std::byte{' '}, std::byte{'S'}, std::byte{'a'}, std::byte{'t'}, std::byte{'i'}, std::byte{'s'}, std::byte{'f'}, std::byte{'i'}, std::byte{'a'}, std::byte{'b'}, std::byte{'l'}, std::byte{'e'}}},
                          {HTTP_STATUS_EXPECTATION_FAILED, {std::byte{'E'}, std::byte{'x'}, std::byte{'p'}, std::byte{'e'}, std::byte{'c'}, std::byte{'t'}, std::byte{'a'}, std::byte{'t'}, std::byte{'i'}, std::byte{'o'}, std::byte{'n'}, std::byte{' '}, std::byte{'F'}, std::byte{'a'}, std::byte{'i'}, std::byte{'l'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_IM_A_TEAPOT, {std::byte{'I'}, std::byte{'\''}, std::byte{'m'}, std::byte{' '}, std::byte{'a'}, std::byte{' '}, std::byte{'T'}, std::byte{'e'}, std::byte{'a'}, std::byte{'p'}, std::byte{'o'}, std::byte{'t'}}},
                          {HTTP_STATUS_UNPROCESSABLE_ENTITY, {std::byte{'U'}, std::byte{'n'}, std::byte{'p'}, std::byte{'r'}, std::byte{'o'}, std::byte{'c'}, std::byte{'e'}, std::byte{'s'}, std::byte{'s'}, std::byte{'a'}, std::byte{'b'}, std::byte{'l'}, std::byte{'e'}, std::byte{' '}, std::byte{'E'}, std::byte{'n'}, std::byte{'t'}, std::byte{'i'}, std::byte{'t'}, std::byte{'y'}}},
                          {HTTP_STATUS_TOO_EARLY, {std::byte{'T'}, std::byte{'o'}, std::byte{'o'}, std::byte{' '}, std::byte{'E'}, std::byte{'a'}, std::byte{'r'}, std::byte{'l'}, std::byte{'y'}}},
                          {HTTP_STATUS_UPGRADE_REQUIRED, {std::byte{'U'}, std::byte{'p'}, std::byte{'g'}, std::byte{'r'}, std::byte{'a'}, std::byte{'d'}, std::byte{'e'}, std::byte{' '}, std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'i'}, std::byte{'r'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_PRECONDITION_REQUIRED, {std::byte{'P'}, std::byte{'r'}, std::byte{'e'}, std::byte{'c'}, std::byte{'o'}, std::byte{'n'}, std::byte{'d'}, std::byte{'i'}, std::byte{'t'}, std::byte{'i'}, std::byte{'o'}, std::byte{'n'},std::byte{' '}, std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'i'}, std::byte{'r'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_TOO_MANY_REQUESTS, {std::byte{'T'}, std::byte{'o'}, std::byte{'o'}, std::byte{' '}, std::byte{'M'}, std::byte{'a'}, std::byte{'n'}, std::byte{'y'}, std::byte{' '}, std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'e'}, std::byte{'s'}, std::byte{'t'}, std::byte{'s'}}},
                          {HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS, {std::byte{'U'}, std::byte{'n'}, std::byte{'a'}, std::byte{'v'}, std::byte{'a'}, std::byte{'i'}, std::byte{'l'}, std::byte{'b'}, std::byte{'l'}, std::byte{'e'}, std::byte{' '}, std::byte{'F'}, std::byte{'o'}, std::byte{'r'}, std::byte{' '}, std::byte{'L'}, std::byte{'e'}, std::byte{'g'}, std::byte{'a'}, std::byte{'l'}, std::byte{' '}, std::byte{'R'}, std::byte{'e'}, std::byte{'a'}, std::byte{'s'}, std::byte{'o'}, std::byte{'n'}, std::byte{'s'}}},
                          {HTTP_STATUS_INTERNAL_SERVER_ERROR, {std::byte{'I'}, std::byte{'n'}, std::byte{'t'}, std::byte{'e'}, std::byte{'r'}, std::byte{'n'}, std::byte{'a'}, std::byte{'l'}, std::byte{' '}, std::byte{'S'}, std::byte{'e'}, std::byte{'r'}, std::byte{'v'}, std::byte{'e'}, std::byte{'r'}, std::byte{' '}, std::byte{'E'}, std::byte{'r'}, std::byte{'r'}, std::byte{'o'}, std::byte{'r'}}},
                          {HTTP_STATUS_NOT_IMPLEMENTED, {std::byte{'N'}, std::byte{'o'}, std::byte{'t'}, std::byte{' '}, std::byte{'I'}, std::byte{'m'}, std::byte{'p'}, std::byte{'l'}, std::byte{'e'}, std::byte{'m'}, std::byte{'e'}, std::byte{'n'}, std::byte{'t'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_BAD_GATEWAY, {std::byte{'B'}, std::byte{'a'}, std::byte{'d'}, std::byte{' '}, std::byte{'G'}, std::byte{'a'}, std::byte{'t'}, std::byte{'e'}, std::byte{'w'}, std::byte{'a'}, std::byte{'y'}}},
                          {HTTP_STATUS_SERVICE_UNAVAILABLE, {std::byte{'S'}, std::byte{'e'}, std::byte{'r'}, std::byte{'v'}, std::byte{'i'}, std::byte{'c'}, std::byte{'e'}, std::byte{' '}, std::byte{'U'}, std::byte{'n'}, std::byte{'a'}, std::byte{'v'}, std::byte{'a'}, std::byte{'i'}, std::byte{'l'}, std::byte{'a'}, std::byte{'b'}, std::byte{'l'}, std::byte{'e'}}},
                          {HTTP_STATUS_GATEWAY_TIMEOUT, {std::byte{'G'}, std::byte{'e'}, std::byte{'t'}, std::byte{'e'}, std::byte{'w'}, std::byte{'a'}, std::byte{'y'}, std::byte{' '}, std::byte{'T'}, std::byte{'i'}, std::byte{'m'}, std::byte{'e'}, std::byte{'o'}, std::byte{'u'}, std::byte{'t'}}},
                          {HTTP_STATUS_VERSION_NOT_SUPPORTED, {std::byte{'V'}, std::byte{'e'}, std::byte{'r'}, std::byte{'s'}, std::byte{'i'}, std::byte{'o'}, std::byte{'n'}, std::byte{' '}, std::byte{'N'}, std::byte{'o'}, std::byte{'t'}, std::byte{' '}, std::byte{'S'}, std::byte{'u'}, std::byte{'p'}, std::byte{'p'}, std::byte{'o'}, std::byte{'r'}, std::byte{'t'}, std::byte{'e'}, std::byte{'d'}}},
                          {HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED, {std::byte{'N'}, std::byte{'e'}, std::byte{'t'}, std::byte{'w'}, std::byte{'o'}, std::byte{'r'}, std::byte{'k'}, std::byte{' '} , std::byte{'A'}, std::byte{'u'}, std::byte{'t'}, std::byte{'h'}, std::byte{'e'}, std::byte{'n'}, std::byte{'t'}, std::byte{'i'}, std::byte{'c'}, std::byte{'a'}, std::byte{'t'}, std::byte{'i'}, std::byte{'o'}, std::byte{'n'}, std::byte{' '}, std::byte{'R'}, std::byte{'e'}, std::byte{'q'}, std::byte{'u'}, std::byte{'i'}, std::byte{'r'}, std::byte{'e'}, std::byte{'d'}}}
                      }),
    _versionMap({
                    {HttpConnectionVersion::HTTP_10, {std::byte{'H'}, std::byte{'T'}, std::byte{'T'}, std::byte{'P'}, std::byte{'/'}, std::byte{'1'}, std::byte{'.'}, std::byte{'0'}}},
                    {HttpConnectionVersion::HTTP_11, {std::byte{'H'}, std::byte{'T'}, std::byte{'T'}, std::byte{'P'}, std::byte{'/'}, std::byte{'1'}, std::byte{'.'}, std::byte{'1'}}}
                }),
    _version(HttpConnectionVersion::UNKNOWN),
    _status(HTTP_STATUS_OK),
    _newLine({std::byte{'\r'}, std::byte{'\n'}}),
    _headerSeparator({std::byte{':'}, std::byte{' '}}),
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
  std::vector<std::byte> data = convertStringToBytes(content);
  sendResponse(data);
}
void HttpResponse::send(std::string &&content, const std::string &contentType)
{
  _headerMap["content-type"] = contentType;
  std::vector<std::byte> data = convertStringToBytes(content);
  sendResponse(data);
}
void HttpResponse::send(const std::string &content, std::string &&contentType)
{
  _headerMap["content-type"] = contentType;
  std::vector<std::byte> data = convertStringToBytes(content);
  sendResponse(data);
}
void HttpResponse::send(std::string &&content, std::string &&contentType)
{
  _headerMap["content-type"] = contentType;
  std::vector<std::byte> data = convertStringToBytes(content);
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
  std::vector<std::byte> jsonData = convertStringToBytes(json);
  sendResponse(std::move(jsonData));
}
void HttpResponse::sendJson(variant &&map)
{
  auto json = json_generator::generate(map);
  std::vector<std::byte> jsonData = convertStringToBytes(json);
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
  if (HttpConnectionVersion::UNKNOWN == _version) {
    _version = HttpConnectionVersion::HTTP_11;
  }

  std::vector<std::byte> response;

  // response status line
  response.insert(response.end(), _versionMap.at(_version).begin(), _versionMap.at(_version).end());
  response.push_back(std::byte{' '});
  auto status = convertHttpResponseStatusToVector(_status);
  response.insert(response.end(), status.begin(), status.end());
  response.insert(response.end(), _statusMessageMap.at(_status).begin(), _statusMessageMap.at(_status).end());
  response.insert(response.end(), _newLine.begin(), _newLine.end());

  // headers
  for (auto &headerPair: _headerMap) {
    std::vector<std::byte> headerKey = convertStringToBytes(headerPair.first);
    std::vector<std::byte> headerValue = convertStringToBytes(headerPair.second);
    response.insert(response.end(), headerKey.begin(), headerKey.end());
    response.insert(response.end(), _headerSeparator.begin(), _headerSeparator.end());
    response.insert(response.end(), headerValue.begin(), headerValue.end());
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
  if (HttpConnectionVersion::UNKNOWN == _version) {
    _version = HttpConnectionVersion::HTTP_11;
  }

  std::vector<std::byte> response;

  // response status line
  response.insert(response.end(), _versionMap.at(_version).begin(), _versionMap.at(_version).end());
  response.push_back(std::byte{' '});
  auto status = convertHttpResponseStatusToVector(_status);
  response.insert(response.end(), status.begin(), status.end());
  response.insert(response.end(), _statusMessageMap.at(_status).begin(), _statusMessageMap.at(_status).end());
  response.insert(response.end(), _newLine.begin(), _newLine.end());

  // headers
  for (auto &headerPair: _headerMap) {
    std::vector<std::byte> headerKey = convertStringToBytes(headerPair.first);
    std::vector<std::byte> headerValue = convertStringToBytes(headerPair.second);
    response.insert(response.end(), headerKey.begin(), headerKey.end());
    response.insert(response.end(), _headerSeparator.begin(), _headerSeparator.end());
    response.insert(response.end(), headerValue.begin(), headerValue.end());
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
  std::vector<std::byte> statusVec = convertStringToBytes(statusStr);
  return statusVec;
}
std::vector<std::byte> HttpResponse::convertStringToBytes(const std::string &data)
{
  std::vector<std::byte> vector;
  std::transform(data.begin(), data.end(), vector.end(), [](auto &c) {
    return std::byte(c);
  });
  return vector;
}
