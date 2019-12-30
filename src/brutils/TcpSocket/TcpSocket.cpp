//
// Created by congay on 07/10/2019.
//

#include "brutils/TcpSocket/TcpSocket.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>

using namespace brutils;

#define DEFAULT_READ_BUFFER_SIZE (1024*1024)
#define INCOMING_DATA_SIZE 1024

TcpSocket::TcpSocket(br_object *parent) :
    TcpSocket(-1, ConnectionStatus::NOT_CONNECTED, DEFAULT_READ_BUFFER_SIZE, parent)
{
}
TcpSocket::TcpSocket(uint64_t readBufferSize, br_object *parent) :
    TcpSocket(-1, ConnectionStatus::NOT_CONNECTED, readBufferSize, parent)
{
}
TcpSocket::TcpSocket(int socketDescriptor, ConnectionStatus status, br_object *parent) :
    TcpSocket(socketDescriptor, status, DEFAULT_READ_BUFFER_SIZE, parent)
{
}
TcpSocket::TcpSocket(int socketDescriptor, ConnectionStatus status, uint64_t readBufferSize, br_object *parent) :
    br_object(parent),
    connected(parent),
    disconnected(parent),
    dataReady(parent),
    errorOccurred(parent),
    destroyed(parent),
    _readBufferSize(readBufferSize),
    _socketD(socketDescriptor),
    _connectionStatus(status)
{
  setReadBufferSize(readBufferSize);
}
TcpSocket::~TcpSocket()
{
  if (ConnectionStatus::CONNECTED == connectionStatus()) {
    disconnect();
  }
  destroyed.emit();
}
ConnectionStatus TcpSocket::connectionStatus() const
{
  std::scoped_lock lock(_mutex);
  return _connectionStatus;
}
TcpError TcpSocket::error() const
{
  std::scoped_lock lock(_mutex);
  return _lastError;
}
std::string TcpSocket::peerAddress()
{
  std::scoped_lock lock(_mutex);
  if (ConnectionStatus::CONNECTED == connectionStatus()) {
    sockaddr_in peer{};
    socklen_t peerLen = sizeof(peer);
    int res = getpeername(_socketD, (sockaddr *) &peer, &peerLen);
    if (0 != res) {
      int errNo = errno;
      _lastError.errorCode = TCP_ERROR_SYS_ERROR;
      _lastError.errorStr.clear();
      _lastError.errorStr.append(std::to_string(errNo));
      _lastError.errorStr.append(" - ");
      _lastError.errorStr.append(strerror(errNo));
      errorOccurred.emit(_lastError);
      return "";
    }
    return inet_ntoa(peer.sin_addr);
  }
  _lastError.errorCode = TCP_ERROR_NOT_ALLOWED_IN_CURRENT_STATE;
  _lastError.errorStr.clear();
  _lastError.errorStr = "Peer address is not available in not connected state";
  errorOccurred.emit(_lastError);
  return "";
}
uint16_t TcpSocket::peerPort()
{
  std::scoped_lock lock(_mutex);
  if (ConnectionStatus::CONNECTED == connectionStatus()) {
    sockaddr_in peer{};
    socklen_t peerLen = sizeof(peer);
    int res = getpeername(_socketD, (sockaddr *) &peer, &peerLen);
    if (0 != res) {
      int errNo = errno;
      _lastError.errorCode = TCP_ERROR_SYS_ERROR;
      _lastError.errorStr.clear();
      _lastError.errorStr.append(std::to_string(errNo));
      _lastError.errorStr.append(" - ");
      _lastError.errorStr.append(strerror(errNo));
      errorOccurred.emit(_lastError);
      return 0;
    }
    return ntohs(peer.sin_port); // NOLINT(hicpp-signed-bitwise)
  }
  _lastError.errorCode = TCP_ERROR_NOT_ALLOWED_IN_CURRENT_STATE;
  _lastError.errorStr.clear();
  _lastError.errorStr = "Peer port is not available in not connected state";
  errorOccurred.emit(_lastError);
  return 0;
}
uint64_t TcpSocket::readBufferSize() const
{
  std::scoped_lock lock(_mutex);
  return _readBufferSize;
}
bool TcpSocket::setReadBufferSize(const uint64_t readBufferSize)
{
  std::scoped_lock lock(_mutex);
  if (!_dataBuffer.empty()) {
    _lastError.errorCode = TCP_ERROR_NOT_ALLOWED_WHILE_BUFFER_IS_NOT_EMPTY;
    _lastError.errorStr.clear();
    _lastError.errorStr = "Cannot modify read buffer size while buffer is not empty";
    errorOccurred.emit(_lastError);
    return false;
  }

  _readBufferSize = std::min(_dataBuffer.max_size(), readBufferSize);
  _dataBuffer.reserve(_readBufferSize);

  return true;
}
int TcpSocket::socketDescriptor() const
{
  std::scoped_lock lock(_mutex);
  return _socketD;
}
bool TcpSocket::setSocketDescriptor(const int sd)
{
  std::scoped_lock lock(_mutex);
  if (ConnectionStatus::NOT_CONNECTED == connectionStatus()) {
    _socketD = sd;
    return true;
  }
  _lastError.errorCode = TCP_ERROR_NOT_ALLOWED_IN_CURRENT_STATE;
  _lastError.errorStr.clear();
  _lastError.errorStr = "Peer address is not available in not connected state";
  errorOccurred.emit(_lastError);
  return false;
}
bool TcpSocket::connect(const std::string address, const uint16_t port) // NOLINT(performance-unnecessary-value-param)
{
  std::scoped_lock lock(_mutex);
  if (ConnectionStatus::NOT_CONNECTED != connectionStatus()) {
    _lastError.errorCode = TCP_ERROR_NOT_ALLOWED_IN_CURRENT_STATE;
    _lastError.errorStr.clear();
    _lastError.errorStr = "Cannot connect while already connected";
    errorOccurred.emit(_lastError);
    return false;
  }
  if (0 < _socketD) {
    _lastError.errorCode = TCP_ERROR_BAD_SOCKET_DESCRIPTOR;
    _lastError.errorStr.clear();
    _lastError.errorStr = "Cannot connected while socked descriptor is valid before connect operation";
    errorOccurred.emit(_lastError);
    return false;
  }

  _socketD = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (-1 == _socketD) {
    int errNo = errno;
    _lastError.errorCode = TCP_ERROR_SYS_ERROR;
    _lastError.errorStr.clear();
    _lastError.errorStr.append(std::to_string(errNo));
    _lastError.errorStr.append(" - ");
    _lastError.errorStr.append(strerror(errNo));
    errorOccurred.emit(_lastError);
    return false;
  }

  struct sockaddr_in addr{};
  inet_pton(AF_INET, address.c_str(), &addr.sin_addr);
  addr.sin_port = port;
  int res = ::connect(_socketD, (sockaddr *) &addr, sizeof(addr));
  if (0 != res) {
    int errNo = errno;
    _lastError.errorCode = TCP_ERROR_SYS_ERROR;
    _lastError.errorStr.clear();
    _lastError.errorStr.append(std::to_string(errNo));
    _lastError.errorStr.append(" - ");
    _lastError.errorStr.append(strerror(errNo));
    errorOccurred.emit(_lastError);
    return false;
  }

  _connectionStatus = ConnectionStatus::CONNECTED;
  connected.emit();
  return true;
}
bool TcpSocket::disconnect()
{
  std::scoped_lock lock(_mutex);
  if (ConnectionStatus::CONNECTED != connectionStatus()) {
    _lastError.errorCode = TCP_ERROR_NOT_ALLOWED_IN_CURRENT_STATE;
    _lastError.errorStr.clear();
    _lastError.errorStr = "Cannot disconnect while already disconnected";
    errorOccurred.emit(_lastError);
    return false;
  }

  if (this->getThreadId() == std::this_thread::get_id()) {
    int res = ::close(_socketD);
    if (0 != res) {
      int errNo = errno;
      _lastError.errorCode = TCP_ERROR_SYS_ERROR;
      _lastError.errorStr.clear();
      _lastError.errorStr.append(std::to_string(errNo));
      _lastError.errorStr.append(" - ");
      _lastError.errorStr.append(strerror(errNo));
      errorOccurred.emit(_lastError);
      return false;
    }
    _connectionStatus = ConnectionStatus::NOT_CONNECTED;
  } else {
    int res = ::shutdown(_socketD, SHUT_RDWR);
    if (0 != res) {
      int errNo = errno;
      _lastError.errorCode = TCP_ERROR_SYS_ERROR;
      _lastError.errorStr.clear();
      _lastError.errorStr.append(std::to_string(errNo));
      _lastError.errorStr.append(" - ");
      _lastError.errorStr.append(strerror(errNo));
      errorOccurred.emit(_lastError);
      return false;
    }
  }

  return true;
}
std::vector<std::byte> TcpSocket::read()
{
  std::scoped_lock lock(_mutex);
  // if standalone
  //  readFromSocket
  // if not
  //  return buffer
  std::vector<std::byte> content(_dataBuffer);
  _dataBuffer.clear();
  return content;
}
bool TcpSocket::readFromSocket()
{
  std::scoped_lock lock(_mutex);
  if (ConnectionStatus::CONNECTED != connectionStatus()) {
    _lastError.errorCode = TCP_ERROR_NOT_ALLOWED_IN_CURRENT_STATE;
    _lastError.errorStr.clear();
    _lastError.errorStr = "Cannot read from socket while not connected";
    errorOccurred.emit(_lastError);
    return false;
  }

  std::byte incomingData[INCOMING_DATA_SIZE];
  ssize_t totalReceivedSize = 0;
  ssize_t incomingDataSize;
  do {
    incomingDataSize = ::recv(_socketD, incomingData, sizeof(incomingData), 0);
    if (-1 == incomingDataSize) {
      int errorNo = errno;
      if (EAGAIN == errorNo || EWOULDBLOCK == errorNo) {
        // no incoming data, no problem
        break;
      } else {
        // socket error
        int errNo = errno;
        _lastError.errorCode = TCP_ERROR_SYS_ERROR;
        _lastError.errorStr.clear();
        _lastError.errorStr.append(std::to_string(errNo));
        _lastError.errorStr.append(" - ");
        _lastError.errorStr.append(strerror(errNo));
        errorOccurred.emit(_lastError);
        break;
      }
    } else if (0 == incomingDataSize) {
      // closed by peer
      disconnect();
      disconnected.emit();
      break;
    } else {
      // read incoming data
      _dataBuffer.insert(_dataBuffer.end(), incomingData, incomingData+incomingDataSize);
      totalReceivedSize += incomingDataSize;
    }
  } while (incomingDataSize == sizeof(incomingData));

  if (0 < totalReceivedSize) {
    // data received
    dataReady.emit();
  }
  return true;
}
bool TcpSocket::write(const std::vector<std::byte> &input)
{
  std::scoped_lock lock(_mutex);
  if (ConnectionStatus::CONNECTED != connectionStatus()) {
    _lastError.errorCode = TCP_ERROR_NOT_ALLOWED_IN_CURRENT_STATE;
    _lastError.errorStr.clear();
    _lastError.errorStr = "Cannot write while not connected";
    errorOccurred.emit(_lastError);
    return false;
  }

  int res = ::send(_socketD, input.data(), input.size(), 0);
  if (0 != res) {
    int errNo = errno;
    _lastError.errorCode = TCP_ERROR_SYS_ERROR;
    _lastError.errorStr.clear();
    _lastError.errorStr.append(std::to_string(errNo));
    _lastError.errorStr.append(" - ");
    _lastError.errorStr.append(strerror(errNo));
    errorOccurred.emit(_lastError);
    return false;
  }
  return true;
}
