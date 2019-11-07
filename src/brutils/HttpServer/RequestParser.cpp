#include "brutils/HttpServer/RequestParser.h"

using namespace brutils;

RequestParser::RequestParser() :
newRequestReady(nullptr),
_parsingStatus(REQUEST_LINE)
{
  _methodMap = {
      {{'G', 'E', 'T'}, GET},
      {{'H', 'E', 'A', 'D'}, HEAD},
      {{'P', 'O', 'S', 'T'}, POST},
      {{'P', 'U', 'T'}, PUT},
      {{'P', 'A', 'T', 'C', 'H'}, PATCH},
      {{'D', 'E', 'L', 'E', 'T', 'E'}, DELETE},
      {{'C', 'O', 'N', 'N', 'E', 'C', 'T'}, CONNECT},
      {{'O', 'P', 'T', 'I', 'O', 'N', 'S'}, OPTIONS},
      {{'T', 'R', 'A', 'C', 'E'}, TRACE}
  };

  _versionMap = {
      {{'H', 'T', 'T', 'P', '/', '1', '.', '0'}, HTTP_10},
      {{'H', 'T', 'T', 'P', '/', '1', '.', '1'}, HTTP_11},
      {{'H', 'T', 'T', 'P', '/', '2', '.', '0'}, HTTP_20},
      {{'H', 'T', 'T', 'P', '/', '3', '.', '0'}, HTTP_30}
  };
}
RequestParser::~RequestParser()
{

}
void RequestParser::newIncomingData(std::vector<uint8_t> &data)
{
  _buffer.insert(_buffer.end(), data.begin(), data.end());
  _content.reset();
  _content = std::make_shared<std::string_view>(_buffer.cbegin(), _buffer.cend());
  parse();
}
void RequestParser::parse()
{
  if (!_requestInProduction) {
    _requestInProduction = std::make_shared<HttpRequest>();
  }

  for (auto pos = _buffer.cbegin(), end = _buffer.cend(); pos != end; ++pos) {
    if (REQUEST_LINE == _parsingStatus) {
      if (!parseRequestLine(pos, end)) {
        _parsingStatus = ERROR;
        break;
      }
    } else if (HEADER_LINES == _parsingStatus) {
      if (!parseHeader(pos, end)) {
        _parsingStatus = ERROR;
      }
    } else if (BODY == _parsingStatus) {
      if (!parseBody(pos, end, 5)) { // _requestInPorduction.header("Content-Size")
        _parsingStatus = ERROR;
      }
    } else if (FINISHED == _parsingStatus) {
      newRequestReady.emit(_requestInProduction);
    } else {
      _parsingStatus = ERROR;
      break;
    }
  }
}
bool RequestParser::parseRequestLine(std::vector<uint8_t>::const_iterator &pos, std::vector<uint8_t>::const_iterator &end)
{
  if (pos == end) {
    return false;
  }



  // read method
  HttpMethod method = UNKNOWN;
  auto it = pos;
  for (; it != end && *it != ' '; ++it); // read until empty space
  for (auto &el: _methodMap) {
    if (std::equal(pos, it, el.first.cbegin(), el.first.cend())) {
      method = el.second;
      break;
    }
  }
  if (UNKNOWN == method) {
    return false;
  }
  if (it == end) {
    return false;
  }
  pos = ++it; // set current position to the last read char (escaping the last empty space)

  // read path (start with iterating past the first empty space till the next empty space)
  for (; it != end && *it != ' '; ++it);
  if (it == end) {
    return false;
  }
  std::string path(pos, it);
  pos = ++it; // set current position to the last read char (escaping the last empty space)

  // read http version
  HttpVersion  version;
  for (; it != end && *it != '\n'; ++it); // read until the end of request line
  for (auto &el: _versionMap) {
    if (std::equal(pos, it, el.first.cbegin(), el.first.cend()) {
      version = el.second;
      break;
    }
  }

}
bool RequestParser::parseHeader(std::vector<uint8_t>::const_iterator &pos, std::vector<uint8_t>::const_iterator &end)
{

}
bool RequestParser::parseBody(std::vector<uint8_t>::const_iterator &pos, std::vector<uint8_t>::const_iterator &end, uint64_t contentSize)
{

}
