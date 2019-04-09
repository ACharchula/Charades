// Copyright 2019 Kamil Mankowski

#include "Server.h"

Server::Server() {
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = 0;
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
  int recivedSize, msgsock;
  Command comm;
  char nextChar;
  if (sockid < 0) return;

  listen(sockid, 5);  // TODO(kmankow): move as const
  while (true) {
    msgsock = accept(sockid, reinterpret_cast<sockaddr *>(0), 0);
    if (msgsock < 0) continue;  // TODO(kmankow): log
    do {
      recivedSize = read(msgsock, &nextChar, 1);
      if (recivedSize > 0) {
        comm.addChar(nextChar);
      }
    } while (recivedSize > 0);
    close(msgsock);
  }
}
