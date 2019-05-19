// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_INTERPRETER_H_
#define SRC_SERVER_INTERPRETER_H_

#include <exception>
#include <string>

#include "commands/Command.h"
#include "commands/EnterTableCmd.h"
#include "commands/HelloCmd.h"
#include "commands/SendMessageCmd.h"
#include "commands/SetCanvasCmd.h"

#include "Table.h"
#include "User.h"

class Interpreter {
 public:
  Interpreter() {}
  explicit Interpreter(User *current_user, Users *users, Tables *tables)
      : current_user(current_user), users(users), tables(tables) {
    setSelectCommandState();
  }
  ~Interpreter();

  void interpretChar(char c);

 private:
  User *current_user = nullptr;
  Users *users;
  Tables *tables;
  // int userid = -1;
  std::string tmp = "";

  int bytesToRead;
  enum ActionState { SelectCommand, ReadLength, PushToCommand } actionState;
  Command *currentCommand = nullptr;

  void proceedInput();

  void setLengthState();
  void setPushState();
  void setSelectCommandState();
};

#endif  // SRC_SERVER_INTERPRETER_H_
