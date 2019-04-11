// Copyright 2019 Kamil Mankowski

#include "GlobalData.h"

std::string GlobalData::popMessage(int userid) {
  if (!isMessageToSend(userid)) return "";
  std::string tmp = msgQueues[userid].front();
  msgQueues[userid].pop();
  return tmp;
}
void GlobalData::addUser(int userid) {
  msgQueues[userid] = std::queue<std::string>();
}
void GlobalData::removeUser(int userid) { msgQueues.erase(userid); }

void GlobalData::addMessageToQueue(int userid, std::string msg) {
  msgQueues[userid].push(msg);
}
