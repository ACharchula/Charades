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

class User;
using user_ptr = std::shared_ptr<User>;

class User {
 public:
  User() {}
  explicit User(int id) : id(id) {}

  struct message {
    enum MsgType { String, Pointer } type;
    std::string str;
    std::vector<char>* point;
  };

  User::message popMessage();
  void addMessageToQueue(std::string msg) {
    messeges.push({message::MsgType::String, msg, nullptr});
  }
  void addMessageToQueue(std::vector<char>* msg) {
    messeges.push({message::MsgType::Pointer, "", msg});
  }
  bool isMessageToSend() { return !messeges.empty(); }

  void login() { logged = true; }
  bool isLogged() { return logged; }

  void setUsername(const std::string& name) { username = name; }
  std::string getUsername() { return username; }
  void setTableId(int id) { table_id = id; }
  int getTableId() { return table_id; }
  int getId() { return id; }

 private:
  int id;
  int table_id;
  std::queue<message> messeges;
  bool logged;
  std::string username;
};

class Users {
 public:
  const std::list<int>& getAllUsersIds() { return userids; }
  void addUser(int userid);
  void removeUser(int userid);
  User* getUser(int id) { return &users[id]; }

 private:
  std::list<int> userids;
  std::map<int, User> users;
};

#endif  // SRC_SERVER_USER_H_
