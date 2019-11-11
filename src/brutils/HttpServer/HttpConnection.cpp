//
// Created by Burak Ongay on 10/11/2019.
//

#include "brutils/HttpServer/HttpConnection.h"

using namespace brutils;

HttpConnection::HttpConnection(std::shared_ptr<TcpSocket> tcpSocket, br_object *parent) :
    br_object(parent),
    newRequestReady(parent),
    tcpErrorOccured(parent),
    tcpSocketDisconnected(parent),
    parseErrorOccured(parent),
    newRequestAvailable(parent),
    responseReady(parent)
{
  tcpErrorOccured.setSlotFunction(
      std::bind(&HttpConnection::tcpErrorOccured_slot, this, std::placeholders::_1));
  tcpSocketDisconnected.setSlotFunction(
      std::bind(&HttpConnection::tcpSocketDisconnected_slot, this));
  parseErrorOccured.setSlotFunction(
      std::bind(&HttpConnection::parseErrorOccured_slot, this, std::placeholders::_1));
  newRequestAvailable.setSlotFunction(
      std::bind(&HttpConnection::newRequestAvailable_slot, this, std::placeholders::_1));
  responseReady.setSlotFunction(
      std::bind(&HttpConnection::responseReady_slot, this));
}
HttpConnection::~HttpConnection()
{

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
  if (HTTP_10 == request->connectionVersion()) {
    // TODO: return error message
  }
  // if upgrade to http 2 is requested in request object, put that logic here

  // create an http response object bind with request and start monitoring it,
  // it's this class' job to provide response mechanism
}
void HttpConnection::responseReady_slot()
{

}
std::shared_ptr<HttpResponse> HttpConnection::createResponse(std::shared_ptr<HttpRequest> request)
{
  auto response = std::make_shared<HttpResponse>();
}
