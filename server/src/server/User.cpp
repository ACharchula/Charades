// Copyright 2019 Kamil Mankowski

#include "User.h"

buffer_ptr User::popMessage() {
  if (!isMessageToSend()) {
    return helpers::to_buf("");
  }
  auto tmp = messeges.front();
  messeges.pop();
  return tmp;
}

void Users::addUser(int userid, int socket) {
  users.insert(std::make_pair(userid, User(userid, socket)));
  userids.push_back(userid);
}

void Users::removeUser(int userid) {
  userids.erase(std::find(userids.begin(), userids.end(), userid));
  users.erase(userid);
}
