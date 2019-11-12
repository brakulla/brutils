//
// Created by Burak Ongay on 10/11/2019.
//

#include "brutils/HttpServer/HttpConnection.h"
#include "brutils/time_utils.h"

using namespace brutils;

#define HTTP2_UPGRADE_HEADER_VALUE "h2c"

HttpConnection::HttpConnection(bool upgradeProtocolInUnsecureConnection,
                               std::shared_ptr<TcpSocket> tcpSocket,
                               br_object *parent) :
    br_object(parent),
    newRequestReady(parent),
    tcpErrorOccured(parent),
    tcpSocketDisconnected(parent),
    parseErrorOccured(parent),
    newRequestAvailable(parent),
    responseReadyToSend(parent),
    _upgradeProtocolInUnsecureConnection(upgradeProtocolInUnsecureConnection),
    _tcpSocket(tcpSocket)
{
  tcpErrorOccured.setSlotFunction(
      std::bind(&HttpConnection::tcpErrorOccured_slot, this, std::placeholders::_1));
  tcpSocketDisconnected.setSlotFunction(
      std::bind(&HttpConnection::tcpSocketDisconnected_slot, this));
  parseErrorOccured.setSlotFunction(
      std::bind(&HttpConnection::parseErrorOccured_slot, this, std::placeholders::_1));
  newRequestAvailable.setSlotFunction(
      std::bind(&HttpConnection::newRequestAvailable_slot, this, std::placeholders::_1));
  responseReadyToSend.setSlotFunction(
      std::bind(&HttpConnection::responseReady_slot, this, std::placeholders::_1));
}
HttpConnection::~HttpConnection()
{
  if (_tcpSocket) {
    _tcpSocket->disconnect();
  }
}
void HttpConnection::tcpErrorOccured_slot(brutils::TcpError error)
{
  // TODO: if tcp error is critical, then self shut this connection
}
void HttpConnection::tcpSocketDisconnected_slot()
{
  // TODO: when tcp socket is disconnected, self shut this connection
}
void HttpConnection::parseErrorOccured_slot(brutils::ParseError error)
{
  // TODO: if parse error is critical, then self shut this connection
}
void HttpConnection::newRequestAvailable_slot(std::shared_ptr<HttpRequest> request)
{
  auto response = createResponse(request);
  if (HTTP_11 != request->connectionVersion()) {
    response->status(HTTP_STATUS_VERSION_NOT_SUPPORTED);
    response->send();
    return;
  }
  // TODO: if upgrade to http 2 is requested in request object, put that logic here
  // it's this class' job to provide response mechanism
  newRequestReady.emit(request, response); // this is the connection to the outer space
}
void HttpConnection::responseReady_slot(std::vector<uint8_t> data)
{
  _tcpSocket->write(data);
}
std::shared_ptr<HttpResponse> HttpConnection::createResponse(std::shared_ptr<HttpRequest> request)
{
  auto response = std::make_shared<HttpServer_private::HttpResponse_private>(this);
  response->readyToWrite.connect(responseReadyToSend);

  response->header("date", getLocaltimeFormatted("%a, %d %m %Y %T %Z"));
  response->header("server", "brutils/cervpp");

  // std::shared_ptr<HttpServer_private::HttpResponse_private> will be coverted to correct return value automatically
  return response;
}
