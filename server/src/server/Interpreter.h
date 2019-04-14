// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_INTERPRETER_H_
#define SRC_SERVER_INTERPRETER_H_

#include <string>
#include "commands/Command.h"
#include "commands/HelloCmd.h"
#include "commands/SendMessageCmd.h"

class Interpreter {
 public:
  Interpreter() {}
  explicit Interpreter(int userid) : userid(userid) {}

  void interpretChar(char c, GlobalData *gdata);

 private:
  int userid = -1;
  std::string tmp = "";

  int bytesToRead = 0;
  enum InputState {
    UntilNewLine,
    GivenBytes
  } inState = InputState::UntilNewLine;
  enum ActionState {
    SelectCommand,
    PushToCommand
  } actionState = ActionState::SelectCommand;
  Command *currentCommand;

  void proceedInput(GlobalData *gdata);
  void setStates(Command::ReturnState rstate);
};

#endif  // SRC_SERVER_INTERPRETER_H_
