// Copyright 2019 Kamil Mankowski

#include "Server.h"

Server::Server(int port) : tables(users) {
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
}

void Server::prepare() {
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) throw std::runtime_error("Error on creating socket!");

  int reuse_yes = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse_yes,
                 sizeof(reuse_yes)) == -1) {
    throw std::runtime_error("Error on sersockopt");
  }

  if (bind(server_socket, reinterpret_cast<sockaddr *>(&address),
           sizeof address) < 0)
    throw std::runtime_error("Error on binding socket!");
}

unsigned int Server::getPort() {
  if (server_socket < 0) return 0;

  length = sizeof(address);
  if (getsockname(server_socket, reinterpret_cast<sockaddr *>(&address),
                  &length) < 0)
    throw std::runtime_error("Error on getting socket name!");
  return ntohs(address.sin_port);
}

void Server::accept_connections() {
  int msgsock;
  bool isNewConnect = FD_ISSET(server_socket, &ready_sockets) &&
                      sockets.size() < MAX_CONNECTIONS;

  if (isNewConnect) {
    msgsock = accept(server_socket, reinterpret_cast<sockaddr *>(0), 0);

    if (msgsock < 0) {
      log("Error on try to accept connection");
    } else if (msgsock > 0) {
      nfds = std::max(nfds, msgsock + 1);
      sockets.push_back(msgsock);
      users.addUser(msgsock, msgsock);
      interpreters.insert(std::make_pair(
          msgsock, Interpreter(users.getUser(msgsock), &users, &tables)));
      log("Accepted connection", msgsock);
    }
  }
}

void Server::recive_datas() {
  int recived, msgsock;
  char buff[BUFFER_SIZE];

  for (auto sock_it = sockets.begin(); sock_it != sockets.end();) {
    msgsock = *sock_it;
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
}

void Server::send_messages() {
  for (auto id : users.getAllUsersIds()) {
    auto user = users.getUser(id);
    while (user->isMessageToSend()) {
      auto msg = user->popMessage();
      if (write(user->sock(), msg->data(), msg->size()) == -1)
        log("Sending binary data error", user->sock());
    }
  }
}

void Server::run() {
  if (server_socket < 0) return;
  run_srv = true;

  tables.createTable();

  listen(server_socket, MAX_WAITING_CONN);
  while (run_srv) {
    FD_ZERO(&ready_sockets);
    FD_SET(server_socket, &ready_sockets);
    for (auto &sock : sockets)
      if (sock > 0) FD_SET(sock, &ready_sockets);

    if (select(nfds, &ready_sockets, 0, 0, &select_timeout) == -1) continue;

    accept_connections();
    recive_datas();
    tables.proceedAll();
    send_messages();
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
  interpreters[usersock].disconnect();
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
  close(server_socket);
  log("Server listening socket closed");
}
