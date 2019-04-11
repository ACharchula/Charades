// Copyright 2019 Kamil Mankowski
// Store information accessible by all

#ifndef SRC_SERVER_GLOBALDATA_H_
#define SRC_SERVER_GLOBALDATA_H_

#include <list>
#include <map>
#include <queue>
#include <string>

class GlobalData {
 public:
  std::list<int> getAllUsers();
  void addMessageToQueue(int userid, std::string msg) {
    msgQueues[userid].push(msg);
  }
  std::string popMessage(int userid);
  bool isMessageToSend(int userid) { return !msgQueues[userid].empty(); }
  void addUser(int userid);
  void removeUser(int userid);

 private:
  std::map<int, std::queue<std::string>> msgQueues;
};

#endif  // SRC_SERVER_GLOBALDATA_H_
