// Copyright 2019 Kamil Mankowski
// Store information accessible by all

#ifndef SRC_SERVER_GLOBALDATA_H_
#define SRC_SERVER_GLOBALDATA_H_

#include <algorithm>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "TableData.h"

class GlobalData {
 public:
  struct message {
    enum MsgType { String, Pointer } type;
    std::string str;
    std::vector<char>* point;
  };

  // GlobalData() {}
  const std::list<int>& getAllUsers() { return userids; }
  void addMessageToQueue(int userid, std::string msg);
  void addMessageToQueue(int userid, std::vector<char>* msg);
  message popMessage(int userid);
  bool isMessageToSend(int userid) { return !msgQueues[userid].empty(); }
  void addUser(int userid);
  void removeUser(int userid);
  void setUsername(int userid, const std::string& username) {
    usernames[userid] = username;
  }
  bool isLogged(int userid) { return logged[userid]; }
  void login(int userid) { logged[userid] = true; }
  std::string getUsername(int userid) { return usernames[userid]; }

  TableData& getTable() { return table; }

 private:
  std::map<int, std::queue<message>> msgQueues;
  std::map<int, std::string> usernames;
  std::map<int, bool> logged;
  std::list<int> userids;

  TableData table;
};

#endif  // SRC_SERVER_GLOBALDATA_H_
