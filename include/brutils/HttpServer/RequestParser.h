#ifndef BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_REQUESTPARSER_H_
#define BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_REQUESTPARSER_H_

#include <memory>
#include <brutils/br_object.h>
#include "brutils/HttpServer/HttpRequest.h"

namespace brutils
{

enum class ParseErrorCode
{
  NoError = 0,
  Method,
  URI,
  Version
};

struct ParseError
{
  ParseErrorCode errorCode;
  std::string errorStr;
};

class RequestParser
{
 public:
  explicit RequestParser(br_object *parent = nullptr) :
      newRequestReady(parent),
      errorOccured(parent),
      newData(parent)
  {}
  virtual ~RequestParser() = default;

  // sync api (should be called until there is no request, even if there is no new data, since there can be multiple requests in the received data)
 public:
  virtual std::shared_ptr<HttpRequest> newIncomingData(std::vector<std::byte> &data) = 0;

  // async api
 public:
  signal<std::shared_ptr<HttpRequest>> newRequestReady;
  signal<ParseError> errorOccured;
  slot<std::vector<std::byte>> newData;
 protected:
  virtual void newDataReceived_slot(std::vector<std::byte> &data) = 0;

};

class RequestParser_v1x : public RequestParser
{
 public:
  explicit RequestParser_v1x(br_object *parent = nullptr);
  ~RequestParser_v1x() override = default;

 public:
  std::shared_ptr<HttpRequest> newIncomingData(std::vector<std::byte> &data) override;

 protected:
  void newDataReceived_slot(std::vector<std::byte> &data) override;

 public:
  // root parse function
  bool parse();
  // first level parse functions
  bool parseRequestLine(std::vector<std::byte>::const_iterator &pos, std::vector<std::byte>::const_iterator &end);
  bool parseHeader(std::vector<std::byte>::const_iterator &pos, std::vector<std::byte>::const_iterator &end);
  bool parseBody(std::vector<std::byte>::const_iterator &pos, std::vector<std::byte>::const_iterator &end, uint64_t contentSize);
  // second level parse functions
  bool parseMethod(std::vector<std::byte>::const_iterator &pos, std::vector<std::byte>::const_iterator &end);
  bool parsePathAndQuery(std::vector<std::byte>::const_iterator &pos, std::vector<std::byte>::const_iterator &end);
  bool parseVersion(std::vector<std::byte>::const_iterator &pos, std::vector<std::byte>::const_iterator &end);
  // third level parse functions
  bool parsePath(std::vector<std::byte>::const_iterator &pos, std::vector<std::byte>::const_iterator &end);
  bool parseQuery(std::vector<std::byte>::const_iterator &pos, std::vector<std::byte>::const_iterator &end);

  std::string getString(std::vector<std::byte>::const_iterator &pos, std::vector<std::byte>::const_iterator &end);

 private:
  std::vector<std::byte > _buffer;
  std::shared_ptr<HttpRequest> _requestInProduction;
  const std::map<std::vector<std::byte>, HttpMethod> _methodMap;
  const std::map<std::vector<std::byte>, HttpVersion> _versionMap;
  const std::vector<std::byte> _newLine;
  const std::vector<std::byte> _emptySpace;

  enum ParsingStatus {
    REQUEST_LINE,
    HEADER_LINES,
    BODY,
    FINISHED,
    ERROR
  } _parsingStatus;
};

}

#endif //BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_REQUESTPARSER_H_
