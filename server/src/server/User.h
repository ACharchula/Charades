// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_USER_H_
#define SRC_SERVER_USER_H_

#include <algorithm>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <utility>
#include <vector>

#include "commands/Helpers.h"

class User;
using user_ptr = std::shared_ptr<User>;

class User {
 public:
  User() {}
  explicit User(int id, int socket) : id(id), socket(socket) {}

  buffer_ptr popMessage();
  void addMessageToQueue(const std::string& msg) {
    messeges.push(helpers::to_buf(msg));
  }
  void addMessageToQueue(buffer_ptr msg) { messeges.push(msg); }
  bool isMessageToSend() { return !messeges.empty(); }

  void login(std::string name) {
    username = name;
    logged = true;
  }
  void logout() { logged = false; }
  bool isLogged() { return logged; }
  int sock() { return socket; }

  // void setUsername(const std::string& name) { username = name; }
  std::string getUsername() { return username; }
  void setTableId(int id) { table_id = id; }
  int getTableId() { return table_id; }
  bool isInTable() { return table_id != NO_TABLE; }
  int getId() { return id; }

  static const int NO_TABLE = -1;

 private:
  int id;
  int socket;
  int table_id = NO_TABLE;
  std::queue<buffer_ptr> messeges;
  bool logged = false;
  std::string username;
};

class Users {
 public:
  const std::list<int>& getAllUsersIds() { return userids; }
  void addUser(int userid, int socket);
  void removeUser(int userid);
  bool loginUser(User* user, std::string name);
  void logoutUser(User* user);
  User* getUser(int id) { return &users[id]; }
  void addWin(std::string name) { userWins[name] += 1; }
  std::vector<std::pair<std::string, int>> getStats();

 private:
  std::list<int> userids;
  std::map<int, User> users;

  std::map<std::string, bool> loginSentinel;
  std::map<std::string, int> userWins;
};

#endif  // SRC_SERVER_USER_H_
