#include "brutils/HttpServer/RequestParser.h"

using namespace brutils;

RequestParser_v1x::RequestParser_v1x(br_object *parent) :
    RequestParser(parent),
    _parsingStatus(REQUEST_LINE),
    _methodMap({
                   {{'G', 'E', 'T'}, GET},
                   {{'H', 'E', 'A', 'D'}, HEAD},
                   {{'P', 'O', 'S', 'T'}, POST},
                   {{'P', 'U', 'T'}, PUT},
                   {{'P', 'A', 'T', 'C', 'H'}, PATCH},
                   {{'D', 'E', 'L', 'E', 'T', 'E'}, DELETE},
                   {{'C', 'O', 'N', 'N', 'E', 'C', 'T'}, CONNECT},
                   {{'O', 'P', 'T', 'I', 'O', 'N', 'S'}, OPTIONS},
                   {{'T', 'R', 'A', 'C', 'E'}, TRACE}
               }),
    _versionMap({
                    {{'H', 'T', 'T', 'P', '/', '1', '.', '0'}, HTTP_10},
                    {{'H', 'T', 'T', 'P', '/', '1', '.', '1'}, HTTP_11}
                }),
    _newLine({{'\r', '\n'}}),
    _emptySpace({' '})
{

}
std::shared_ptr<HttpRequest> RequestParser_v1x::newIncomingData(std::vector<std::byte> &data)
{
  _buffer.insert(_buffer.end(), data.begin(), data.end());
  if (parse()) {
    auto request = _requestInProduction;
    _requestInProduction.reset();
    return request;
  }
  return nullptr;
}
void RequestParser_v1x::newDataReceived_slot(std::vector<std::byte> &data)
{
  _buffer.insert(_buffer.end(), data.begin(), data.end());
  while (true) {
    bool requestAvailable = parse();
    if (requestAvailable) {
      newRequestReady.emit(_requestInProduction);
      _requestInProduction.reset();
    } else {
      break;
    }
  }
}
bool RequestParser_v1x::parse()
{
  if (!_requestInProduction) {
    _requestInProduction = std::make_shared<HttpServer_private::HttpRequest_private>();
  }

  for (auto pos = _buffer.cbegin(), end = _buffer.cend(); pos != end;) {
    // !all parsing functions used below are greedy!
    if (REQUEST_LINE == _parsingStatus) {
      if (!parseRequestLine(pos, end)) {
        _parsingStatus = ERROR;
      } else {
        _parsingStatus = HEADER_LINES;
      }
    }
    if (HEADER_LINES == _parsingStatus) {
      if (!parseHeader(pos, end)) {
        _parsingStatus = ERROR;
      } else {
        std::string contentSize = _requestInProduction->header("content-size");
        if (contentSize.empty()) {
          _parsingStatus = FINISHED;
        } else {
          _parsingStatus = BODY;
        }
      }
    }
    if (BODY == _parsingStatus) {
      uint64_t contentSize = std::stoll(_requestInProduction->header("content-size"));
      if (!parseBody(pos, end, contentSize)) {
        _parsingStatus = ERROR;
      } else {
        _parsingStatus = FINISHED;
      }
    }

    if (FINISHED == _parsingStatus) {
      _parsingStatus = REQUEST_LINE;
//      newRequestReady.emit(_requestInProduction);
      return true;
    }
    if (ERROR == _parsingStatus) {
      errorOccured.emit(ParseError()); // send error data
      return false;
    }
  }

  return false;
}
bool RequestParser_v1x::parseRequestLine(std::vector<std::byte>::const_iterator &pos,
                                         std::vector<std::byte>::const_iterator &end)
{
  if (pos == end) {
    return false;
  }

  // get first line
  auto lineEndPos = std::find_first_of(pos, end, _newLine.cbegin(), _newLine.cend());

  // read method
  if (!parseMethod(pos, lineEndPos)) {
    ParseError error {
        PARSE_ERROR_REQUESTLINE_METHOD,
        "Cannot parse method in request line"
    };
    errorOccured.emit(error);
    return false;
  }

  // read path and query
  if (!parsePathAndQuery(pos, lineEndPos)) {
    ParseError error {
        PARSE_ERROR_REQUESTLINE_URI,
        "Cannot parse uri in request line"
    };
    return false;
  }

  // read http protocol version
  if (!parseVersion(pos, lineEndPos)) {
    ParseError error {
        PARSE_ERROR_REQUESTLINE_VERSION,
        "Cannot parse version in request line"
    };
    return false;
  }

  // remove new line characters
  std::advance(pos, _newLine.size());
  return true;
}
bool RequestParser_v1x::parseHeader(std::vector<std::byte>::const_iterator &pos,
                                    std::vector<std::byte>::const_iterator &end)
{
  while (pos != end) {
    // find end of line
    // check length of line
    //    if zero, finished, return
    //    else process
    //      if while processing it comes to the end of data, return false
    auto lineEndPos = std::find_first_of(pos, end, _newLine.cbegin(), _newLine.end());
    if (0 == std::distance(pos, lineEndPos)) { // processed all header lines
      std::advance(pos, 2); // progress position by two characters to reach to the next line
      return true;
    }
    auto keyEndPos = std::find(pos, lineEndPos, ':');
    if (keyEndPos == lineEndPos) { // ':' not found
      return false;
    }
    std::string key(pos, keyEndPos);
    pos = keyEndPos;
    std::advance(pos, 1); // advance ahead of ':'
    auto valueStart = std::find_if(pos, lineEndPos, [] (std::byte el) {
      return el != ' ';
    });
    std::string value(valueStart, lineEndPos);
    pos = lineEndPos;
    std::advance(pos, 2); // advance ahead of "\r\n"
    std::static_pointer_cast<HttpServer_private::HttpRequest_private>(
        _requestInProduction
        )->insertHeader(key, value);
  }
  // correct processing should be finished within while loop, so if flow reached here, it is an error
  return false;
}
bool RequestParser_v1x::parseBody(std::vector<std::byte>::const_iterator &pos,
                                  std::vector<std::byte>::const_iterator &end,
                                  uint64_t contentSize)
{
  if (contentSize > std::distance(pos, end)) {
    return false;
  }
  std::vector<std::byte> body(pos, end);
  pos = end;
  std::static_pointer_cast<HttpServer_private::HttpRequest_private>(_requestInProduction)->setRawBody(body);
  // TODO: use body parser here or somewhere else after this point and set meaningful body of request
  return true;
}
bool RequestParser_v1x::parseMethod(std::vector<std::byte>::const_iterator &pos,
                                    std::vector<std::byte>::const_iterator &end)
{
  auto methodEndPos = std::find(pos, end, ' '); // find next empty space to separate method
  if (methodEndPos == end) { // we couldn't find empty space, something wrong
    return false;
  }
  HttpRequestMethod method = UNKNOWN_METHOD;
  for (auto &el: _methodMap) {
    if (std::equal(pos, methodEndPos, el.first.cbegin(), el.first.cend())) {
      method = el.second;
      break;
    }
  }
  if (UNKNOWN_METHOD == method) {
    return false;
  }
  pos = methodEndPos;
  std::advance(pos, 1); // advance ahead of empty space
  std::static_pointer_cast<HttpServer_private::HttpRequest_private>(_requestInProduction)->setMethod(method);
  return true;
}
bool RequestParser_v1x::parsePathAndQuery(std::vector<std::byte>::const_iterator &pos, std::vector<std::byte>::const_iterator &end)
{
  auto uriEndPos = std::find(pos, end, ' ');
  if (uriEndPos == end) {
    return false;
  }
  auto queryStartPos = std::find(pos, uriEndPos, '?');
  if (queryStartPos == uriEndPos) {
    parsePath(pos, uriEndPos);
  } else {
    if (!parsePath(pos, queryStartPos)) {
      return false;
    }
    std::advance(pos, 1); // advance ahead of '?' char
    if (!parseQuery(pos, uriEndPos)) {
      return false;
    }
  }
  return true;
}
bool RequestParser_v1x::parseVersion(std::vector<std::byte>::const_iterator &pos,
                                     std::vector<std::byte>::const_iterator &end)
{
  // read http version, this time, we read until the end of the line
  HttpConnectionVersion version = UNKNOWN_VERSION;
  for (auto &el: _versionMap) {
    if (std::equal(pos, end, el.first.cbegin(), el.first.cend())) {
      version = el.second;
      break;
    }
  }
  if (UNKNOWN_VERSION == version) {
    return false;
  }
  pos = end;
  std::static_pointer_cast<HttpServer_private::HttpRequest_private>(_requestInProduction)->setVersion(version);
  return true;
}
bool RequestParser_v1x::parsePath(std::vector<std::byte>::const_iterator &pos,
                                         std::vector<std::byte>::const_iterator &end)
{
  if (0 >= std::distance(pos, end)) {
    return false;
  }
  std::string path(pos, end);
  pos = end;
  std::static_pointer_cast<HttpServer_private::HttpRequest_private>(_requestInProduction)->setPath(path);
  return true;
}
bool RequestParser_v1x::parseQuery(std::vector<std::byte>::const_iterator &pos,
                                   std::vector<std::byte>::const_iterator &end)
{
  std::map<std::string, std::string> query;
  while(0 < std::distance(pos, end)) {
    auto pairEndPos = std::find(pos, end, '&');
    auto keyEndPos = std::find(pos, pairEndPos, '=');
    if (keyEndPos == pairEndPos) {
      query.emplace(std::string(pos, pairEndPos), "");
    } else {
      auto valueStartPos = keyEndPos;
      std::advance(valueStartPos, 1);
      query.emplace(std::string(pos, keyEndPos), std::string(valueStartPos, pairEndPos));
    }
    pos = pairEndPos;
    std::advance(pos, 1); // advance ahead of '&' or '='
  }
  std::static_pointer_cast<HttpServer_private::HttpRequest_private>(_requestInProduction)->setQuery(query);
  return true;
}
