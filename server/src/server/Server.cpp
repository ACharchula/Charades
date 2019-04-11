// Copyright 2019 Kamil Mankowski

#include "Server.h"

Server::Server() {
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = 0;

  for (auto &sock : sockets) sock = 0;
}

void Server::prepare() {
  sockid = socket(AF_INET, SOCK_STREAM, 0);
  if (sockid < 0) throw std::runtime_error("Error on creating socket!");

  if (bind(sockid, reinterpret_cast<sockaddr *>(&address), sizeof address) < 0)
    throw std::runtime_error("Error on binding socket!");
}

unsigned int Server::getPort() {
  if (sockid < 0) return 0;

  length = sizeof(address);
  if (getsockname(sockid, reinterpret_cast<sockaddr *>(&address), &length) < 0)
    throw std::runtime_error("Error on getting socket name!");
  return ntohs(address.sin_port);
}

void Server::run() {
  int recived, msgsock = -1, actives;
  Command comm;
  char nextChar;
  if (sockid < 0) return;

  listen(sockid, 5);  // TODO(kmankow): move as const
  while (true) {
    FD_ZERO(&ready_sockets);
    FD_SET(sockid, &ready_sockets);
    for (auto &sock : sockets)
      if (sock > 0) FD_SET(sock, &ready_sockets);

    if ((actives = select(nfds, &ready_sockets, 0, 0, &select_timeout)) == -1)
      log("Select error, continuing...");

    if (FD_ISSET(sockid, &ready_sockets) && sockets.size() < MAX_CONNECTIONS) {
      msgsock = accept(sockid, reinterpret_cast<sockaddr *>(0), 0);
      if (msgsock < 0) {
        log("Error on try to accept connection");

      } else if (msgsock > 0) {
        nfds = std::max(nfds, msgsock + 1);
        sockets.push_back(msgsock);
        log("Accepted connection", msgsock);
      }
    }

    for (auto sock_it = sockets.begin(); sock_it != sockets.end();) {
      int msgsock = *sock_it;
      if (msgsock > 0 && FD_ISSET(msgsock, &ready_sockets)) {
        recived = read(msgsock, &nextChar, 1);

        if (recived == -1) {
          log("Error on reading", msgsock);
        } else if (recived == 0) {
          close(msgsock);
          sock_it = sockets.erase(sock_it);
          log("Close connection", msgsock);
          continue;
        } else {
          // comm.addChar(nextChar);
          std::cout << "[" << msgsock << "] " << nextChar << std::endl;
        }
      }
      ++sock_it;
    }
  }
}

void Server::log(const std::string &msg, int sock) {
  std::cout << "SYSLOG [" << sock << "]: " << msg << std::endl;
}
