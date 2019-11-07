#ifndef BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_REQUESTPARSER_H_
#define BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_REQUESTPARSER_H_

#include <brutils/br_object.h>
#include <string_view>
#include <memory>

namespace brutils
{

class HttpRequest {

};

enum HttpMethod {
  GET,
  HEAD,
  POST,
  PUT,
  PATCH,
  DELETE,
  CONNECT,
  OPTIONS,
  TRACE,
  UNKNOWN
};

enum HttpVersion {
  HTTP_10,
  HTTP_11,
  HTTP_20,
  HTTP_30
};

class RequestParser
{
 public:
  RequestParser();
  ~RequestParser();

 public: // signals
  signal<std::shared_ptr<HttpRequest>> newRequestReady;

 public:
  void newIncomingData(std::vector<uint8_t> &data);

 private:
  void parse();
  bool parseRequestLine(std::vector<uint8_t>::const_iterator &pos, std::vector<uint8_t>::const_iterator &end);
  bool parseHeader(std::vector<uint8_t>::const_iterator &pos, std::vector<uint8_t>::const_iterator &end);
  bool parseBody(std::vector<uint8_t>::const_iterator &pos, std::vector<uint8_t>::const_iterator &end, uint64_t contentSize);

 private:
  std::vector<uint8_t > _buffer;
  std::shared_ptr<std::string_view> _content;
  std::shared_ptr<HttpRequest> _requestInProduction;
  std::map<std::vector<uint8_t>, HttpMethod> _methodMap;
  std::map<std::vector<uint8_t>, HttpVersion> _versionMap;

  enum ParsingStatus {
    REQUEST_LINE,
    HEADER_LINES,
    BODY,
    FINISHED,
    ERROR
  } _parsingStatus;
};

// G -> GET
// H -> HEAD
// P -> POST
//      PUT
//      PATCH
// D -> DELETE
// C -> CONNECT
// O -> OPTIONS
// T -> TRACE

}

#endif //BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_REQUESTPARSER_H_
