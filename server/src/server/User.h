// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_USER_H_
#define SRC_SERVER_USER_H_

#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "commands/Helpers.h"

class User;
using user_ptr = std::shared_ptr<User>;

class User {
 public:
  User() {}
  explicit User(int id, int socket) : id(id), socket(socket) {}

  buffer_ptr popMessage();
  void addMessageToQueue(const std::string &msg) {
    messeges.push(helpers::to_buf(msg));
  }
  void addMessageToQueue(buffer_ptr msg) { messeges.push(msg); }
  bool isMessageToSend() { return !messeges.empty(); }

  void login() { logged = true; }
  bool isLogged() { return logged; }
  int sock() { return socket; }

  void setUsername(const std::string& name) { username = name; }
  std::string getUsername() { return username; }
  void setTableId(int id) { table_id = id; }
  int getTableId() { return table_id; }
  int getId() { return id; }

  static const int NO_TABLE = -1;

 private:
  int id;
  int socket;
  int table_id = NO_TABLE;
  std::queue<buffer_ptr> messeges;
  bool logged;
  std::string username;
};

class Users {
 public:
  const std::list<int>& getAllUsersIds() { return userids; }
  void addUser(int userid, int socket);
  void removeUser(int userid);
  User* getUser(int id) { return &users[id]; }

 private:
  std::list<int> userids;
  std::map<int, User> users;
};

#endif  // SRC_SERVER_USER_H_
