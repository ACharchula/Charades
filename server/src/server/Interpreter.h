// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_INTERPRETER_H_
#define SRC_SERVER_INTERPRETER_H_

#include <exception>
#include <memory>
#include <utility>
#include <string>

#include "commands/Command.h"
#include "commands/EnterTableCmd.h"
#include "commands/HelloCmd.h"
#include "commands/SendMessageCmd.h"
#include "commands/SetCanvasCmd.h"
#include "commands/ComeOutTableCmd.h"

#include "Table.h"
#include "User.h"

class Interpreter {
 public:
  Interpreter() {}
  Interpreter(User *current_user, Users *users, Tables *tables);

  void interpretChar(char c);
  void disconnect();

 private:
  User *current_user = nullptr;
  Users *users;
  Tables *tables;
  buffer_ptr tmp;

  int bytesToRead;
  enum ActionState { SelectCommand, ReadLength, PushToCommand } actionState;
  command_ptr currentCommand = nullptr;

  void proceedInput();

  void setLengthState();
  void setPushState();
  void setSelectCommandState();

  bool equal(buffer_ptr left, buffer_ptr right) {
    return std::equal(left->begin(), left->end(), right->begin());
  }
};

#endif  // SRC_SERVER_INTERPRETER_H_
