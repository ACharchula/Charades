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
  if (users[userid].isLogged()) {
    logoutUser(&users[userid]);
  }
  userids.erase(std::find(userids.begin(), userids.end(), userid));
  users.erase(userid);
}

bool Users::loginUser(User* user, std::string name) {
  if (loginSentinel.find(name) != loginSentinel.end()) {
    if (loginSentinel[name]) return false;
  }
  loginSentinel[name] = true;
  if (userWins.count(name) == 0) userWins[name] = 0;
  user->login(name);
  return true;
}

void Users::logoutUser(User* user) {
  if (user->isLogged()) {
    loginSentinel[user->getUsername()] = false;
  }
  user->logout();
}

std::vector<std::pair<std::string, int>> Users::getStats() {
  std::vector<std::pair<std::string, int>> out;
  for (auto el : userWins) out.push_back(el);
  return out;
}
