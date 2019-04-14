// Copyright 2019 Kamil Mankowski
// Store information accessible by all

#ifndef SRC_SERVER_GLOBALDATA_H_
#define SRC_SERVER_GLOBALDATA_H_

#include <algorithm>
#include <list>
#include <map>
#include <queue>
#include <string>

class GlobalData {
 public:
  const std::list<int>& getAllUsers() { return userids; }
  void addMessageToQueue(int userid, std::string msg);
  std::string popMessage(int userid);
  bool isMessageToSend(int userid) { return !msgQueues[userid].empty(); }
  void addUser(int userid);
  void removeUser(int userid);

 private:
  std::map<int, std::queue<std::string>> msgQueues;
  std::list<int> userids;
};

#endif  // SRC_SERVER_GLOBALDATA_H_
