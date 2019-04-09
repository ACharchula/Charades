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

 private:
  int sockid = -1;
  socklen_t length;
  struct sockaddr_in address;
};

#endif  // SRC_SERVER_SERVER_H_
