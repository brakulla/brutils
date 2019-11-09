//
// Created by brakulla on 24.02.2019.
//
#include <iostream>
#include <thread>
#include <any>
#include <fstream>

#include "brutils/HttpServer/RequestParser.h"

int main()
{
  std::cout << "Hello, World!" << std::endl;

  brutils::RequestParser_v1x parser;

  std::cout << "Query test" << std::endl;

  std::vector<uint8_t> queryString {
//    'p', 'a', 'r', 's', 'i', 'n', 'g', '=', 'w', 'o', 'r', 'k', 'i', 'n', 'g'
//      'p', 'a', 'r', 's', 'i', 'n', 'g'
//      'p', 'a', 'r', 's', 'i', 'n', 'g', '=', 'w', 'o', 'r', 'k', 'i', 'n', 'g', '&', 'a', 'n', 'd', 't', 'h', 'e',
//      's', 'e', 'c', 'o', 'n', 'd', 'k', 'e', 'y', 'o', 'n', 'l', 'y'
      'p', 'a', 'r', 's', 'i', 'n', 'g', '=', 'w', 'o', 'r', 'k', 'i', 'n', 'g', '&', 'a', 'n', 'd', 't', 'h', 'e', 's',
      'e', 'c', 'o', 'n', 'd', 'k', 'e', 'y', 'o', 'n', 'l', 'y', '=', 'a', 'n', 'd', 't', 'h', 'e', 'v', 'a', 'l', 'u',
      'e'
  };
  auto pos = queryString.cbegin();
  auto end = queryString.cend();
  parser.parseQuery(pos, end);

  std::cout << "Path test" << std::endl;

  std::string path = "/just/an/api";
  std::vector<uint8_t> pathString(path.cbegin(), path.cend());
  auto posPath = pathString.cbegin();
  auto endPath = pathString.cend();
  parser.parsePath(posPath, endPath);

  std::cout << "Uri test" << std::endl;

  std::string uri = "/just/an/api?just=an&api ";
  std::vector<uint8_t> uriString(uri.cbegin(), uri.cend());
  auto posUri = uriString.cbegin();
  auto endUri = uriString.cend();
  parser.parsePathAndQuery(posUri, endUri);

  std::cout << "Method test" << std::endl;

  std::string method = "GET " + uri;
  std::vector<uint8_t> methodString(method.cbegin(), method.cend());
  auto posMethod = methodString.cbegin();
  auto endMethod = methodString.cend();
  parser.parseMethod(posMethod, endMethod);

  std::cout << "Version test" << std::endl;

  std::string version = "HTTP/1.0";
  std::vector<uint8_t> versionString(version.cbegin(), version.cend());
  auto posVersion = versionString.cbegin();
  auto endVersion = versionString.cend();
  parser.parseVersion(posVersion, endVersion);

  std::cout << "RequestLine test" << std::endl;

  std::string requestLine = "GET /just/an/api?just=an&api HTTP/1.1\r\n";
  std::vector<uint8_t> requestLineString(requestLine.cbegin(), requestLine.cend());
  auto posRL = requestLineString.cbegin();
  auto endRL = requestLineString.cend();
  parser.parseRequestLine(posRL, endRL);


  std::string header = "Content-Type: Something sthm\r\nAccept: anything actually\r\nAnotherHeader: header\r\n\r\n";
  std::vector<uint8_t> headerString(header.cbegin(), header.cend());
  auto posHeader = headerString.cbegin();
  auto endHeader = headerString.cend();
  parser.parseHeader(posHeader, endHeader);

  std::cout << "Goodbye, World!" << std::endl;
  return 0;
}