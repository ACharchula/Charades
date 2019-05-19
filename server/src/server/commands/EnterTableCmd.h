// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_ENTERTABLECMD_H_
#define SRC_SERVER_COMMANDS_ENTERTABLECMD_H_

#include <string>

#include "Command.h"

class EnterTableCmd : public Command {
 public:
  using Command::Command;
  // explicit EnterTableCmd(int userid) : Command(userid) {}
  void pushInput(std::string input) override;

  static const char HEADER[];

 private:
  static const char update_header[];
};

#endif  // SRC_SERVER_COMMANDS_ENTERTABLECMD_H_
