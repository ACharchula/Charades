// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_INTERPRETER_H_
#define SRC_SERVER_INTERPRETER_H_

#include <exception>
#include <string>

#include "commands/Command.h"
#include "commands/HelloCmd.h"
#include "commands/SendMessageCmd.h"
#include "commands/EnterTableCmd.h"
#include "commands/SetCanvasCmd.h"

class Interpreter {
 public:
  Interpreter() {}
  explicit Interpreter(int userid) : userid(userid) { setSelectCommandState(); }
  ~Interpreter();

  void interpretChar(char c, GlobalData *gdata);

 private:
  int userid = -1;
  std::string tmp = "";

  int bytesToRead;
  enum ActionState {
    SelectCommand,
    PushToCommand
  } actionState;
  Command *currentCommand = nullptr;

  void proceedInput(GlobalData *gdata);

  void setPushState();
  void setSelectCommandState();
};

#endif  // SRC_SERVER_INTERPRETER_H_
