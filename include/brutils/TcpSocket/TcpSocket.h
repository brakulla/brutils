

#ifndef TCPSOCKET_TCPSOCKET_TCPSOCKET_H_
#define TCPSOCKET_TCPSOCKET_TCPSOCKET_H_

#include <string>
#include <mutex>

#include "brutils/br_object.hpp"

namespace brutils {

enum class ConnectionStatus
{
  NOT_CONNECTED,
  CONNECTED
};

enum TcpError_e
{
  NO_ERROR = 0,
  SYS_ERROR,
  NOT_ALLOWED_IN_CURRENT_STATE,
  NOT_ALLOWED_WHILE_BUFFER_IS_NOT_EMPTY,
  BAD_SOCKET_DESCRIPTOR
};

struct TcpError
{
  TcpError_e errorCode;
  std::string errorStr;
};

class TcpSocket : public br_object
{
 public:
  explicit TcpSocket(br_object *parent = nullptr);
  explicit TcpSocket(uint64_t readBufferSize, br_object *parent = nullptr);
  explicit TcpSocket(int socketDescriptor, ConnectionStatus status, br_object *parent = nullptr);
  explicit TcpSocket(int socketDescriptor,
                     ConnectionStatus status,
                     uint64_t readBufferSize,
                     br_object *parent = nullptr);
  ~TcpSocket();

 public: // signals:
  signal<> connected;
  signal<> disconnected;
  signal<> dataReady;
  signal<TcpError> errorOccurred;
  signal<> destroyed;

 public:
  ConnectionStatus connectionStatus() const;
  TcpError error() const;

  std::string peerAddress();
  uint16_t peerPort();

  uint64_t readBufferSize() const;
  bool setReadBufferSize(uint64_t readBufferSize);

  int socketDescriptor() const;
  bool setSocketDescriptor(int sd);

 public:
  bool connect(std::string address, uint16_t port);
  bool disconnect();

  std::vector<uint8_t> read();
  bool write(const std::string &input);
  bool readFromSocket();

 private:
  ConnectionStatus _connectionStatus;
  TcpError _lastError;
  uint64_t _readBufferSize;
  std::vector<uint8_t> _dataBuffer;
  int _socketD;

  mutable std::recursive_mutex _mutex;
};

} // namespace brutils

#endif //TCPSOCKET_TCPSOCKET_TCPSOCKET_H_
