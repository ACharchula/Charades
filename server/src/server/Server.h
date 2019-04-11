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

#include "Command.h"

class Server {
 public:
  Server();
  void run();
  void prepare();
  unsigned int getPort();

  static const int MAX_CONNECTIONS = 100;

 private:
  int sockid = -1;  // TODO(kamman): rename to server_socket
  int nfds = 0;
  socklen_t length;
  struct sockaddr_in address;

  int sockets[MAX_CONNECTIONS + 1];
  fd_set ready_sockets;

  timeval select_timeout = {5, 0};
};

#endif  // SRC_SERVER_SERVER_H_
