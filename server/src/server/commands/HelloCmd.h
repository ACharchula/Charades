// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_HELLOCMD_H_
#define SRC_SERVER_COMMANDS_HELLOCMD_H_

#include <string>

#include "Command.h"

class HelloCmd : public Command {
 public:
  using Command::Command;
  // explicit HelloCmd(int userid) : Command(userid) {}
  void pushInput(std::string input) override;

  static const char HEADER[];

 private:
  static const char output_packet[];
};

#endif  // SRC_SERVER_COMMANDS_HELLOCMD_H_
