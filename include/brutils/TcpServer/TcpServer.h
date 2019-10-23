//
// Created by congay on 15/10/2019.
//

#ifndef BRUTILS_INCLUDE_BRUTILS_TCPSERVER_TCPSERVER_H_
#define BRUTILS_INCLUDE_BRUTILS_TCPSERVER_TCPSERVER_H_

#define DEFAULT_MAX_CONNECTION_SIZE (INT16_MAX-1)
#define DEFAULT_CONNECTION_TIMEOUT_MSEC (180*1000) /* 180 seconds - 115 seconds on Firefox*/
#define DEFAULT_POLL_TIMEOUT_MSEC (500) /* 500 msecs */

#include "brutils/br_object.h"
#include "brutils/TcpSocket/TcpSocket.h"
#include "brutils/timers/combined_timer.h"

#include <thread>
#include <functional>
#include <memory>
#include <map>
#include <set>
#include <poll.h>

namespace brutils
{

enum class TcpServerState
{
  INACTIVE,
  STARTING,
  LISTENING,
  PROCESSING,
  STOPPING
};

class TcpServer : br_object
{
 public:
  explicit TcpServer(br_object *parent = nullptr);
  TcpServer(uint16_t maxConnectionSize,
            uint64_t connectionTimeout_msec,
            uint64_t readBufferSize,
            br_object *parent = nullptr);
  ~TcpServer() override;

 public: // signals
  signal<std::shared_ptr<TcpSocket>> newIncomingConnection;
 private: // slots
  slot<int> timeoutOccured;

 public:
  bool listen(uint16_t port, std::string address = "");
  bool stop();
  bool waitForFinished();

 private:
  void run();

  bool processSockets();

  bool acceptNewConnections();
  void timeoutOnSocket(int16_t timerId);

  bool addNewConnection(int socketDescriptor);
  bool newIncomingData(int socketDescriptor);
  bool connectionClosed(int socketDescriptor);

  bool addToSocketList(int socketFd);
  bool removeFromSocketList(int socketFd);

  bool addToActiveConnections(std::shared_ptr<TcpSocket> &socket);
  std::shared_ptr<TcpSocket> getActiveConnection(int socketDescriptor);
  bool removeFromActiveConnections(int socketDescriptor);

  bool startTimeoutTimer(int socketDescriptor);
  bool stopTimeoutTimer(int socketDescriptor);
  bool stopTimeoutTimer(uint16_t timerId);

 private:
  TcpServerState _serverState;
  int _serverSD;
  int _maxConnectionSize;
  uint64_t _connectionTimeout;
  uint64_t _readBufferSize;

  pollfd *_socketList;
  combined_timer _timer;
  std::set<int> _socketListEmptyIndexSet;
  std::map<int, int> _socketFdIndexMap; // <socketFd, socketListIndex>
  std::map<int, uint16_t> _socketFdTimerIdMap; // <timerId, socketFd> \ these two are complementary
  std::map<uint16_t, int> _timerIdSocketFdMap; // <socketFd, timerId> /        maps, two way map thing
  std::map<int, std::shared_ptr<TcpSocket>> _activeConnections; // <socketFd, TcpSocket>

 private:
  std::atomic_bool _running;
  std::thread _thread;

};

} // namespace brutils

#endif //BRUTILS_INCLUDE_BRUTILS_TCPSERVER_TCPSERVER_H_
