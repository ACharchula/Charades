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

#include "Table.h"

class GlobalData {
 public:
  struct message {
    enum MsgType { String, Pointer } type;
    std::string str;
    std::vector<char> *point;
  };

  const std::list<int>& getAllUsers() { return userids; }
  void addMessageToQueue(int userid, std::string msg);
  void addMessageToQueue(int userid, std::vector<char>* msg);
  message popMessage(int userid);
  bool isMessageToSend(int userid) { return !msgQueues[userid].empty(); }
  void addUser(int userid);
  void removeUser(int userid);

  Table& getTable() { return table; }

 private:
  std::map<int, std::queue<message>> msgQueues;
  std::list<int> userids;

  Table table;
};

#endif  // SRC_SERVER_GLOBALDATA_H_
