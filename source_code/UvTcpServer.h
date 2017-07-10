#ifndef UVTCPSERVER_H
#define UVTCPSERVER_H

#define TCP_PORT 23232
#define TCP_BACKLOG 128

#include "Log.h"
#include "HiveProtocol.h"
#include "UsrData.h"
#include "GlobalData.h"

#include <QThread>
#include <QDebug>
#include <QHash>

#include <uv.h>

class UvTcpServer;
class Bee;

class UvTcpServer : public QThread
{
  Q_OBJECT

  typedef int SocketDescriptor;
  typedef struct {
    uv_write_t req;
    uv_buf_t buf;
  } write_req_t;

public:
  explicit UvTcpServer(QObject *parent = 0);
  ~UvTcpServer();

protected:
  void run();

private:
  static HiveProtocol *hive_protocol;
  static uv_loop_t *loop;
  static struct sockaddr_in addr;

  static QHash<SocketDescriptor, Bee*> bee_hash;
  static QHash<SocketDescriptor, UsrData*> usr_data_hash;

  inline static void onNewConnection(uv_stream_t *server, int status);
  inline static void tcpRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
  inline static void tcpWrite(uv_write_t *req, int status);
  inline static void allocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
  inline static void freeWriteReq(uv_write_t *req);

  inline static int getSocketDescriptor(uv_stream_t *client);

};


class Bee
{
public:
  explicit Bee(uv_stream_t *tcpHandle, const int &fd);

  bool read(const QString &data);
  bool write(const QString & data);

  bool isLeaving();
  bool isIdentified();

private:
  enum MessageType {
    FileInfo = 0,
    FileContent = 1,
    FileAccept = 2,
    FileReject = 3,
    ErrorDelivery = 6,
  };

  int socket_descriptor;
  uv_stream_t *tcp_handle;
  UsrData *usr_data = nullptr;
  QString buffer;
  int read_size = 0;

  bool is_leaving;

  inline bool decodePacket(const QString &data);

};

#endif // UVTCPSERVER_H