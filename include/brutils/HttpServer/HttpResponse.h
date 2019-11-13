//
// Created by congay on 11/11/2019.
//

#ifndef BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPRESPONSE_H_
#define BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPRESPONSE_H_

#include <filesystem>
#include "brutils/br_object.h"
#include "brutils/variant.h"
#include "HttpRequest.h"

namespace brutils
{

enum HttpResponseStatus {
  // TODO: implement the logic for internal status codes (rfc-7231)
  HTTP_STATUS_CONTINUE = 100,
  HTTP_STATUS_SWITCHING_PROTOCOL = 101,
  HTTP_STATUS_OK = 200,
  HTTP_STATUS_CREATED = 201,
  HTTP_STATUS_ACCEPTED = 202,
  HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION = 203,
  HTTP_STATUS_NO_CONTENT = 204,
  HTTP_STATUS_RESET_CONTENT = 205,
  HTTP_STATUS_PARTIAL_CONTENT = 206,
  HTTP_STATUS_MULTIPLE_CHOICES = 300,
  HTTP_STATUS_MOVED_PERMANENTLY = 301,
  HTTP_STATUS_FOUND = 302,
  HTTP_STATUS_SEE_OTHER = 303,
  HTTP_STATUS_NOT_MODIFIED = 304,
  HTTP_STATUS_USE_PROXY = 305,
  HTTP_STATUS_TEMPORARY_REDIRECT = 307,
  HTTP_STATUS_PERMANENT_REDIRECT = 308,
  HTTP_STATUS_BAD_REQUEST = 400,
  HTTP_STATUS_UNAUTHORIZED = 401,
  HTTP_STATUS_PAYMENT_REQUIRED = 402,
  HTTP_STATUS_FORBIDDEN = 403,
  HTTP_STATUS_NOT_FOUND = 404,
  HTTP_STATUS_METHOD_NOT_ALLOWED = 405,
  HTTP_STATUS_NOT_ACCEPTABLE = 406,
  HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED = 407,
  HTTP_STATUS_REQUEST_TIMEOUT = 408,
  HTTP_STATUS_CONFLICT = 409,
  HTTP_STATUS_GONE = 410,
  HTTP_STATUS_LENGTH_REQUIRED = 411,
  HTTP_STATUS_PRECONDITION_FAILED = 412,
  HTTP_STATUS_PAYLOAD_TOO_LARGE = 413,
  HTTP_STATUS_URI_TOO_LONG = 414,
  HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE = 415,
  HTTP_STATUS_RANGE_NOT_SATISFIABLE = 416,
  HTTP_STATUS_EXPECTATION_FAILED = 417,
  HTTP_STATUS_IM_A_TEAPOT = 418,
  HTTP_STATUS_UNPROCESSABLE_ENTITY = 422,
  HTTP_STATUS_TOO_EARLY = 425,
  HTTP_STATUS_UPGRADE_REQUIRED = 426,
  HTTP_STATUS_PRECONDITION_REQUIRED = 428,
  HTTP_STATUS_TOO_MANY_REQUESTS = 431,
  HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS = 451,
  HTTP_STATUS_INTERNAL_SERVER_ERROR = 500,
  HTTP_STATUS_NOT_IMPLEMENTED = 501,
  HTTP_STATUS_BAD_GATEWAY = 502,
  HTTP_STATUS_SERVICE_UNAVAILABLE = 503,
  HTTP_STATUS_GATEWAY_TIMEOUT = 504,
  HTTP_STATUS_VERSION_NOT_SUPPORTED = 505,
  HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED = 511
};

class HttpResponse : public br_object
{
 public:
  explicit HttpResponse(br_object *parent = nullptr);
  ~HttpResponse() override = default;

 public:
  void header(const std::string &key, const std::string &value);
  void header(std::string &&key, const std::string &value);
  void header(const std::string &key, std::string &&value);
  void header(std::string &&key, std::string &&value);
  std::string getHeader(const std::string &key);
  std::string getHeader(std::string &&key);
  void removeHeader(const std::string &key);
  void removeHeader(std::string &&key);
  void clearHeaders();

  void status(const HttpResponseStatus &status);
  void status(HttpResponseStatus &&status);
  void status(const int &status);
  void status(int &&status);

  void send();
  void send(const std::string &content, const std::string &contentType);
  void send(std::string &&content, const std::string &contentType);
  void send(const std::string &content, std::string &&contentType);
  void send(std::string &&content, std::string &&contentType);
  void send(const std::vector<std::byte> &content, const std::string &contentType);
  void send(std::vector<std::byte> &&content, const std::string &contentType);
  void send(const std::vector<std::byte> &content, std::string &&contentType);
  void send(std::vector<std::byte> &&content, std::string &&contentType);
  void sendJson(const variant &map);
  void sendJson(variant &&map);
  void sendXml(const variant &map);
  void sendXml(variant &&map);
  void sendFile(const std::filesystem::path &filePath);
  void sendFile(std::filesystem::path &&filePath);

 protected:
  HttpConnectionVersion _version;
  HttpResponseStatus _status;
  std::map<HttpResponseStatus, std::vector<std::byte>> _statusMessageMap;
  std::map<HttpConnectionVersion, std::vector<std::byte>> _versionMap;
  std::map<std::string, std::string> _headerMap;
  std::vector<std::byte> _newLine;
  std::vector<std::byte> _headerSeparator;

  std::vector<std::byte> convertHttpResponseStatusToVector(HttpResponseStatus status);
  void sendResponse(const std::vector<std::byte> &responseData);
  void sendResponse(std::vector<std::byte> &&responseData);

 protected:
  bool _privateT;
};

namespace HttpServer_private {
class HttpResponse_private : public HttpResponse
{
 public:
  explicit HttpResponse_private(br_object *parent) :
      HttpResponse(parent),
      readyToWrite(parent)
  {
    _privateT = true;
  }

  signal<std::vector<std::byte>> readyToWrite;
};
}

}

#endif //BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPRESPONSE_H_
