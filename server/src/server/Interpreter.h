// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_INTERPRETER_H_
#define SRC_SERVER_INTERPRETER_H_

#include <chrono>
#include <ctime>
#include <exception>
#include <memory>
#include <string>
#include <utility>

#include "commands/ComeOutTableCmd.h"
#include "commands/Command.h"
#include "commands/CreateTableCmd.h"
#include "commands/EnterTableCmd.h"
#include "commands/GetStatisticCmd.h"
#include "commands/GiveUpCmd.h"
#include "commands/HelloCmd.h"
#include "commands/ListTablesCmd.h"
#include "commands/PongCmd.h"
#include "commands/SendMessageCmd.h"
#include "commands/SetCanvasCmd.h"

#include "Tables.h"
#include "User.h"

class Interpreter {
 public:
  Interpreter() {}
  Interpreter(User *current_user, Users *users, Tables *tables);

  void interpretChar(char c);
  void disconnect();
  bool isActive();

 private:
  User *current_user = nullptr;
  Users *users;
  Tables *tables;
  buffer_ptr tmp;
  std::chrono::time_point<std::chrono::system_clock> last_active;
  bool waitForPong = false;

  int bytesToRead;
  enum ActionState { SelectCommand, ReadLength, PushToCommand } actionState;
  command_ptr currentCommand = nullptr;

  void proceedInput();
  void setActive();

  void checkCommands();
  void checkPreloginCommands();
  void checkLogedCommands();
  void checkTableCommands();

  void setLengthState();
  void setPushState();
  void setSelectCommandState();

  bool equal(buffer_ptr left, buffer_ptr right) {
    return std::equal(left->begin(), left->end(), right->begin());
  }

  static const std::chrono::seconds INACTIVE_PING_SEC;
  static const buffer_ptr PING_PACKET;
};

#endif  // SRC_SERVER_INTERPRETER_H_
