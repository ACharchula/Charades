// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_INTERPRETER_H_
#define SRC_SERVER_INTERPRETER_H_

#include <queue>
#include <string>

class Interpreter {
 public:
  Interpreter() {}
  explicit Interpreter(int userid) : userid(userid) {}

  void interpretChar(char c);
  void addMessageToQueue(std::string msg) { msgQueue.push(msg); }
  std::string popMessage();
  bool isMessageToSend() { return !msgQueue.empty(); }

 private:
  std::queue<std::string> msgQueue;
  int userid = -1;
};

#endif  // SRC_SERVER_INTERPRETER_H_
