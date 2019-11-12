//
// Created by Burak Ongay on 10/11/2019.
//

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

 private: // private slots
  slot<TcpError> tcpErrorOccured;
  slot<> tcpSocketDisconnected;
  slot<ParseError> parseErrorOccured;
  slot<std::shared_ptr<HttpRequest>> newRequestAvailable;
  slot<std::vector<uint8_t>> responseReadyToSend;

 private:
  void tcpErrorOccured_slot(TcpError error);
  void tcpSocketDisconnected_slot();
  void parseErrorOccured_slot(ParseError error);
  void newRequestAvailable_slot(std::shared_ptr<HttpRequest> request);
  void responseReady_slot(std::vector<uint8_t> data);

  std::shared_ptr<HttpResponse> createResponse(std::shared_ptr<HttpRequest> request);

 private: // configuration
  bool _upgradeProtocolInUnsecureConnection;

 private:
  std::unique_ptr<RequestParser> _parser;
  std::shared_ptr<TcpSocket> _tcpSocket;

};

}

#endif //BRUTILS_INCLUDE_BRUTILS_HTTPSERVER_HTTPCONNECTION_H_
