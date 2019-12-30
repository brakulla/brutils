/**
 * @file HttpConnection.h
 * @class HttpConnection
 *
 * @brief Virtual HTTP Connection handler.
 *
 * The logic of HTTP connections are implemented in this class.
 * If HTTP 1.1 version is used, then default "Connection" header is "keep-alive". If stated otherwise, this class
 * will self-destruct after one request/response messaging.
 * If HTTP 2.0 version is used, not implemented, yet.
 */

#ifndef BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPCONNECTION_H_
#define BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPCONNECTION_H_

#include "brutils/br_object.h"
#include "brutils/TcpSocket/TcpSocket.h"
#include "brutils/HttpServer/RequestParser.h"
#include "brutils/HttpServer/HttpResponse.h"

namespace brutils
{

class HttpConnection : public br_object
{
 public:
  explicit HttpConnection(bool upgradeProtocolInUnsecureConnection,
                          std::shared_ptr<TcpSocket> tcpSocket,
                          br_object *parent = nullptr);
  ~HttpConnection() override;

 public: // signals
  signal<std::shared_ptr<HttpRequest>, std::shared_ptr<HttpResponse>> newRequestReady;
  signal<std::vector<std::byte>> rawDataReceived;
  signal<> connectionClosed;

 private: // private slots
  // tcp socket connections
  slot<TcpError> tcp_errorOccured;
  slot<> tcp_disconnected;
  slot<> tcp_dataReceived;

  // request parser connections
  slot<ParseError> parser_errorOccured;
  slot<std::shared_ptr<HttpRequest>> parser_newRequest;
  slot<std::vector<std::byte>> responseReadyToSend;

 private:
  void tcp_errorOccured_slot(TcpError error);
  void tcp_disconnected_slot();
  void tcp_dataReceived_slot();

  void parseErrorOccured_slot(ParseError error);
  void newRequestAvailable_slot(std::shared_ptr<HttpRequest> request);
  void responseReady_slot(std::vector<std::byte> data);

  std::shared_ptr<HttpResponse> createResponse(std::shared_ptr<HttpRequest> request);

 private: // configuration
  bool _upgradeProtocolInUnsecureConnection;

 private:
  std::unique_ptr<RequestParser> _parser;
  std::shared_ptr<TcpSocket> _tcpSocket;

};

}

#endif //BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPCONNECTION_H_
