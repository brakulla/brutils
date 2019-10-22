//
// Created by congay on 15/10/2019.
//

#include "brutils/TcpServer/TcpServer.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <link.h>

using namespace brutils;

TcpServer::TcpServer(br_object *parent) :
    br_object(parent),
    newIncomingConnection(parent),
    timeoutOccured(parent),
    _maxConnectionSize(DEFAULT_MAX_CONNECTION_SIZE),
    _connectionTimeout(DEFAULT_CONNECTION_TIMEOUT_MSEC),
    _readBufferSize(0),
    _serverSD(-1),
    _serverState(TcpServerState::INACTIVE),
    _running(false)
{

}
TcpServer::TcpServer(uint16_t maxConnectionSize,
                     uint64_t connectionTimeout_msec,
                     uint64_t readBufferSize,
                     br_object *parent) :
    br_object(parent),
    newIncomingConnection(parent),
    timeoutOccured(parent),
    _maxConnectionSize(maxConnectionSize),
    _connectionTimeout(connectionTimeout_msec),
    _readBufferSize(readBufferSize),
    _serverSD(-1),
    _serverState(TcpServerState::INACTIVE),
    _running(false)
{

}
TcpServer::~TcpServer()
{
  if (_running) {
    _running = false;
  }
  if (_thread.joinable()) {
    _thread.join();
  }
  delete[] _socketList;
}
bool TcpServer::listen(uint16_t port, std::string address)
{
  _serverState = TcpServerState::STARTING;

  _serverSD = ::socket(AF_INET, SOCK_STREAM, 0);
  if (9 == _serverSD) {
    // TODO: set error
    return false;
  }

  int opt = 1;
  if (0 != ::setsockopt(_serverSD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    // TODO: set error
    return false;
  }

  int flags = ::fcntl(_serverSD, F_GETFL);
  if (0 < fcntl(_serverSD, F_SETFL, flags | O_NONBLOCK)) {
    // TODO: set error
    return false;
  }

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
  if (address.empty()) {
    serverAddr.sin_addr.s_addr = INADDR_ANY;
  } else {
    int res = ::inet_pton(AF_INET, address.c_str(), &(serverAddr.sin_addr));
    if (res != 0) {
      // TODO: set error
      return false;
    }
  }

  int bindRes = ::bind(_serverSD, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  if (0 > bindRes) {
    // TODO: set error
    return false;
  }

  int listenRes = ::listen(_serverSD, _maxConnectionSize);
  if (0 > listenRes) {
    // TODO: set error
    return false;
  }

  // connect timeout for sockets
  timeoutOccured.setSlotFunction(std::bind(&TcpServer::timeoutOnSocket, this, std::placeholders::_1));

  _running = true;
  _thread = std::thread(&TcpServer::run, this);
}
bool TcpServer::stop()
{
  _serverState = TcpServerState::STOPPING;
  _running = false;
}
bool TcpServer::waitForFinished()
{
  if (_thread.joinable()) {
    _thread.join();
  }
}
void TcpServer::run()
{
  _socketList = new pollfd[_maxConnectionSize];
  _socketList[0].fd = _serverSD;
  _socketList[0].events = POLLIN;

  for (int i = 1; i < _maxConnectionSize; ++i) {
    _socketList[i].fd = -1;
    _socketList[i].events = POLLIN;
    _socketListEmptyIndexSet.insert(i);
  }

  std::map<int, int> socketIndexMap; // to get index of a socket descriptor

  while (_running) {
    _serverState = TcpServerState::LISTENING;
    int pollRes = ::poll(_socketList, _maxConnectionSize, DEFAULT_POLL_TIMEOUT_MSEC);
    if (0 > pollRes) {
      // TODO: set error
      return;
    } else if (0 == pollRes) {
      // timeout
      continue;
    } else {
      processSockets();
    }
  }
}
bool TcpServer::processSockets()
{
  _serverState = TcpServerState::PROCESSING;
  // server socket
  if (POLLIN == _socketList[0].revents) {
    if (!acceptNewConnections()) {
      return false;
    }
  } else if (0 == _socketList->revents) {
    // nothing happened on server socket, continue
  } else {
    // TODO: set error
    return false;
  }

  // client sockets
  for (auto &it : _socketFdIndexMap) {
    int socketD = it.first;
    int index = it.second;

    if (socketD != _socketList[index].fd) {
      // TODO: set error;
      return false;
    }

    if (0 == _socketList[index].revents) {
      // no events on this socket, continue
      continue;
    }

    // NOTE: data received and connection closed events can happen at the same time
    // check incoming data
    bool dataReceived = false;
    if (POLLIN == (_socketList[index].revents & POLLIN)) {
      if (!newIncomingData(_socketList[index].fd)) {
        // TODO: notify error
      } else {
        dataReceived = true;
      }
    }

    // check connection close event
    if (POLLHUP == (_socketList[index].revents & POLLHUP)) {
      if (!connectionClosed(_socketList[index].fd)) {
        // TODO: notify error
      }
    } else {
      if (!dataReceived) {
        // TODO: set error
        // TODO: notify error
      }
    }
  }
}
bool TcpServer::acceptNewConnections()
{
  if (_socketListEmptyIndexSet.empty()) {
    // TODO: set error (max connection size)
    // logic: wait until a connection is closed
    return false;
  }

  // NOTE: there can be multiple incoming connections, hence the while loop
  while (_running) {
    sockaddr_in client{};
    socklen_t sockLen = sizeof(client);
    int newSocket = ::accept(_socketList[0].fd, (sockaddr *) &client, &sockLen);
    if (0 < newSocket) {
      if (!addNewConnection(newSocket)) {
        // TODO: set error
        continue;
      }
    } else if (-1 == newSocket) {
      // NOTE: no more incoming connections, break the loop
      break;
    } else if (0 >= newSocket) {
      // TODO: set error
      return false;
    }
  }
}
void TcpServer::timeoutOnSocket(int16_t timerId)
{
  auto it = _timerIdSocketFdMap.find(timerId);
  if (_timerIdSocketFdMap.end() == it) {
    // TODO: set error
    return;
  }

  int socketDescriptor = _timerIdSocketFdMap.at(timerId);
  auto item = _activeConnections.find(socketDescriptor);
  if (_activeConnections.end() == item) {
    // TODO: set error
    return;
  }
  std::shared_ptr<TcpSocket> tcpSocket = item->second;
  tcpSocket->disconnect(); // this should trigger related revents in poll
}
bool TcpServer::addNewConnection(int socketDescriptor)
{
  if (_socketListEmptyIndexSet.empty()) {
    // TODO: set error
    return false;
  }

  // TODO: give configuration parameters to TcpSocket below
  std::shared_ptr<TcpSocket> tcpSocket;
  if (_readBufferSize > 0) {
    tcpSocket = std::make_shared<TcpSocket>(socketDescriptor, ConnectionStatus::CONNECTED, _readBufferSize, this);
  } else {
    tcpSocket = std::make_shared<TcpSocket>(socketDescriptor, ConnectionStatus::CONNECTED, this);
  }
  if (!addToSocketList(socketDescriptor)) {
    // TODO: set error
    return false;
  }
  if (!addToActiveConnections(tcpSocket)) {
    // TODO: set error
    return false;
  }
  startTimeoutTimer(socketDescriptor);
}
bool TcpServer::newIncomingData(int socketDescriptor)
{
  auto it = _activeConnections.find(socketDescriptor);
  if (_activeConnections.end() == it) {
    // TODO: set error
    return false;
  }
  auto connection = it->second;

  stopTimeoutTimer(socketDescriptor);
  connection->readFromSocket();
  startTimeoutTimer(socketDescriptor);
}
bool TcpServer::connectionClosed(int socketDescriptor)
{
  removeFromSocketList(socketDescriptor);
  stopTimeoutTimer(socketDescriptor);
  // TODO: notify disconnection
  removeFromActiveConnections(socketDescriptor);
}
bool TcpServer::addToSocketList(int socketFd)
{
  if (_socketListEmptyIndexSet.empty()) {
    // TODO: set error
    return false;
  }

  int firstIndex = *(_socketListEmptyIndexSet.begin());
  _socketList[firstIndex].fd = socketFd;
  _socketList[firstIndex].events = POLLIN;
  _socketFdIndexMap[socketFd] = firstIndex;
  _socketListEmptyIndexSet.erase(firstIndex);
}
bool TcpServer::removeFromSocketList(int socketFd)
{
  auto element = _socketFdIndexMap.find(socketFd);
  if (_socketFdIndexMap.end() == element) {
    // TODO: set error
    return false;
  }
  _socketList[element->second].fd = -1;
  _socketListEmptyIndexSet.insert(element->second);
  _socketFdIndexMap.erase(element);
}
bool TcpServer::addToActiveConnections(std::shared_ptr<TcpSocket> socket)
{
  if (_activeConnections.end() != _activeConnections.find(socket->socketDescriptor())) {
    // TODO: set error
    return false;
  }
  _activeConnections[socket->socketDescriptor()] = socket;
  return true;
}
std::shared_ptr<TcpSocket> TcpServer::getActiveConnection(int socketDescriptor)
{
  auto it = _activeConnections.find(socketDescriptor);
  if (_activeConnections.end() == it) {
    // TODO: set error
    return nullptr;
  }
  return it->second;
}
bool TcpServer::removeFromActiveConnections(int socketDescriptor)
{
  auto it = _activeConnections.find(socketDescriptor);
  if (_activeConnections.end() == it) {
    // TODO: set error
    return false;
  }
  _activeConnections.erase(socketDescriptor);
  return true;
}
bool TcpServer::startTimeoutTimer(int socketDescriptor)
{
  int16_t id = _timer.addTimer(_connectionTimeout);
  _socketFdTimerIdMap[socketDescriptor] = id;
  _timerIdSocketFdMap[id] = socketDescriptor;
  return true;
}
bool TcpServer::stopTimeoutTimer(int socketDescriptor)
{
  auto it = _socketFdTimerIdMap.find(socketDescriptor);
  if (_socketFdTimerIdMap.end() == it) {
    // TODO: set error
    return false;
  }
  int16_t id = it->second;
  if (!_timer.stopTimer(id)) {
    // TODO: set error
    return false;
  }
  _socketFdTimerIdMap.erase(socketDescriptor);
  _timerIdSocketFdMap.erase(id);
  return true;
}
bool TcpServer::stopTimeoutTimer(uint16_t timerId)
{
  auto it = _timerIdSocketFdMap.find(timerId);
  if (_timerIdSocketFdMap.end() == it) {
    // TODO: set error
    return false;
  }
  if (!_timer.stopTimer(timerId)) {
    // TODO: set error
    return false;
  }
  int socketDescriptor = it->second;
  _timerIdSocketFdMap.erase(timerId);
  _socketFdTimerIdMap.erase(socketDescriptor);
}