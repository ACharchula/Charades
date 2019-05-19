// Copyright 2019 Kamil Mankowski

#include "Server.h"

Server::Server(int port) : tables(users) {
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
}

void Server::prepare() {
  sockid = socket(AF_INET, SOCK_STREAM, 0);
  if (sockid < 0) throw std::runtime_error("Error on creating socket!");

  int reuse_yes = 1;
  if (setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, &reuse_yes,
                 sizeof(reuse_yes)) == -1) {
    throw std::runtime_error("Error on sersockopt");
  }

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
  if (sockid < 0) return;
  run_srv = true;

  int recived, msgsock = -1, actives;
  char buff[BUFFER_SIZE];

  listen(sockid, 5);  // TODO(kmankow): move as const
  while (run_srv) {
    FD_ZERO(&ready_sockets);
    FD_SET(sockid, &ready_sockets);
    for (auto &sock : sockets)
      if (sock > 0) FD_SET(sock, &ready_sockets);

    if ((actives = select(nfds, &ready_sockets, 0, 0, &select_timeout)) == -1)
      continue;

    if (FD_ISSET(sockid, &ready_sockets) && sockets.size() < MAX_CONNECTIONS) {
      msgsock = accept(sockid, reinterpret_cast<sockaddr *>(0), 0);
      if (msgsock < 0) {
        log("Error on try to accept connection");
      } else if (msgsock > 0) {
        nfds = std::max(nfds, msgsock + 1);
        sockets.push_back(msgsock);
        users.addUser(msgsock);
        interpreters.insert(std::make_pair(
            msgsock, Interpreter(users.getUser(msgsock), &users, &tables)));
        log("Accepted connection", msgsock);
      }
    }

    for (auto sock_it = sockets.begin(); sock_it != sockets.end();) {
      int msgsock = *sock_it;
      if (msgsock > 0 && FD_ISSET(msgsock, &ready_sockets)) {
        recived = read(msgsock, &buff, BUFFER_SIZE);

        if (recived == -1) {
          log("Error on reading", msgsock);
        } else if (recived == 0) {
          disconnect(msgsock);
          sock_it = sockets.erase(sock_it);
          continue;
        } else {
          try {
            for (int i = 0; i < recived; ++i)
              interpreters[msgsock].interpretChar(buff[i]);
          } catch (const std::exception &e) {
            log("Error on interpreting", msgsock);
            disconnect(msgsock);
            sock_it = sockets.erase(sock_it);
            continue;
          }
        }
      }
      ++sock_it;
    }

    tables.proceedAll();

    for (auto msgsock : sockets) {
      auto user = users.getUser(msgsock);
      while (user->isMessageToSend()) {
        auto msg = user->popMessage();
        if (msg.type == User::message::MsgType::String) {
          if (write(msgsock, msg.str.c_str(), msg.str.size()) == -1)
            log("Sending string error", msgsock);
        } else {
          if (write(msgsock, msg.point->data(), msg.point->size()) == -1)
            log("Sending binary data error", msgsock);
        }
      }
    }
  }

  close_srv();
  log("Server stopped.");
}

void Server::log(const std::string &msg, int sock) {
  std::cout << "SERVER_LOG";
  if (sock >= 0) std::cout << " [" << sock << "]";
  std::cout << ": " << msg << std::endl;
}

void Server::disconnect(int usersock) {
  close(usersock);
  interpreters.erase(usersock);
  users.removeUser(usersock);
  log("Client disconnected", usersock);
}

void Server::stop() {
  log("SIGNAL: STOP SERVER.");
  run_srv = false;
}

void Server::close_srv() {
  for (auto sock : sockets)
    if (sock > 0) disconnect(sock);
  close(sockid);
  log("Server listening socket closed");
}
