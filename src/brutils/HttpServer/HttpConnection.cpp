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
    tcp_errorOccured(parent),
    tcp_disconnected(parent),
    tcp_dataReceived(parent),
    parser_errorOccured(parent),
    parser_newRequest(parent),
    responseReadyToSend(parent),
    _upgradeProtocolInUnsecureConnection(upgradeProtocolInUnsecureConnection),
    _tcpSocket(tcpSocket)
{
  tcp_errorOccured.setSlotFunction(
      std::bind(&HttpConnection::tcp_errorOccured_slot, this, std::placeholders::_1));
  tcp_disconnected.setSlotFunction(
      std::bind(&HttpConnection::tcp_disconnected_slot, this));
  tcp_dataReceived.setSlotFunction(
      std::bind(&HttpConnection::tcp_dataReceived_slot, this));
  parser_errorOccured.setSlotFunction(
      std::bind(&HttpConnection::parseErrorOccured_slot, this, std::placeholders::_1));
  parser_newRequest.setSlotFunction(
      std::bind(&HttpConnection::newRequestAvailable_slot, this, std::placeholders::_1));
  responseReadyToSend.setSlotFunction(
      std::bind(&HttpConnection::responseReady_slot, this, std::placeholders::_1));

  _tcpSocket->errorOccurred.connect(tcp_errorOccured);
  _tcpSocket->disconnected.connect(tcp_disconnected);
  _tcpSocket->dataReady.connect(tcp_dataReceived);

  _parser->errorOccured.connect(parser_errorOccured);
  _parser->newRequestReady.connect(parser_newRequest);
}
HttpConnection::~HttpConnection()
{
  if (_tcpSocket) {
    _tcpSocket->disconnect();
  }
}
void HttpConnection::tcp_errorOccured_slot(brutils::TcpError error)
{
  // TODO: if tcp error is critical, then self shut this connection
}
void HttpConnection::tcp_disconnected_slot()
{
  // TODO: when tcp socket is disconnected, self shut this connection
}
void HttpConnection::tcp_dataReceived_slot()
{
  // TODO: read all data, forward it to request parser
}
void HttpConnection::parseErrorOccured_slot(brutils::ParseError error)
{
  // TODO: if parse error is critical, then self shut this connection
}
void HttpConnection::newRequestAvailable_slot(std::shared_ptr<HttpRequest> request)
{
  auto response = createResponse(request);
  if (HttpVersion::HTTP_11 != request->connectionVersion()) {
    response->status(HTTP_STATUS_VERSION_NOT_SUPPORTED);
    response->send();
    return;
  }
  // TODO: if upgrade to http 2 is requested in request object, put that logic here when it's implemented
  // it's this class' job to provide response mechanism
  newRequestReady.emit(request, response); // this is the connection to the outer space
}
void HttpConnection::responseReady_slot(std::vector<std::byte> data)
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
