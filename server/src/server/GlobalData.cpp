// Copyright 2019 Kamil Mankowski

#include "GlobalData.h"

GlobalData::message GlobalData::popMessage(int userid) {
  if (!isMessageToSend(userid)) {
    return {message::MsgType::String, "", nullptr};
  }
  auto tmp = msgQueues[userid].front();
  msgQueues[userid].pop();
  return tmp;
}
void GlobalData::addUser(int userid) {
  msgQueues[userid] = std::queue<message>();
  userids.push_back(userid);
}
void GlobalData::removeUser(int userid) {
  userids.erase(std::find(userids.begin(), userids.end(), userid));
  msgQueues.erase(userid);
}

void GlobalData::addMessageToQueue(int userid, std::string msg) {
  msgQueues[userid].push({message::MsgType::String, msg, nullptr});
}

void GlobalData::addMessageToQueue(int userid, std::vector<char>* msg) {
  msgQueues[userid].push({message::MsgType::Pointer, "", msg});
}
