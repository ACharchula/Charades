// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_SENDMESSAGECMD_H_
#define SRC_SERVER_COMMANDS_SENDMESSAGECMD_H_

#include <string>

#include "Command.h"

class SendMessageCmd : public Command {
 public:
  using Command::Command;
  // explicit SendMessageCmd(int userid) : Command(userid) {}
  void pushInput(std::string input) override;

  static const char HEADER[];

 private:
  static const char output_header[];
};

#endif  // SRC_SERVER_COMMANDS_SENDMESSAGECMD_H_
