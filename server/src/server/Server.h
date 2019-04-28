// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_SERVER_H_
#define SRC_SERVER_SERVER_H_

#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdexcept>

#include <iostream>
#include <list>
#include <map>
#include <string>

#include "GlobalData.h"
#include "Interpreter.h"
#include "TableMgmt.h"

class Server {
 public:
  explicit Server(int port = 0);
  void run();
  void prepare();
  void stop();
  unsigned int getPort();

  static const int MAX_CONNECTIONS = 100;
  static const int BUFFER_SIZE = 1024;

 private:
  bool run_srv = false;
  int sockid = -1;  // TODO(kamman): rename to server_socket
  int nfds = 0;
  socklen_t length;
  struct sockaddr_in address;

  GlobalData gdata;
  TableMgmt tmgmt;
  std::list<int> sockets;
  std::map<int, Interpreter> interpreters;
  fd_set ready_sockets;

  timeval select_timeout = {5, 0};

  void log(const std::string &msg, int sock = -1);
  void disconnect(int usersock);
  void close_srv();
};

#endif  // SRC_SERVER_SERVER_H_
