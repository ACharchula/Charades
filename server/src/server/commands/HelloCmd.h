// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_HELLOCMD_H_
#define SRC_SERVER_COMMANDS_HELLOCMD_H_

#include <string>

#include "Command.h"

class HelloCmd : public Command {
 public:
  using Command::Command;
  void pushInput(buffer_ptr input) override;

  static const buffer_ptr HEADER;

 private:
  static const buffer_ptr output_packet;
};

#endif  // SRC_SERVER_COMMANDS_HELLOCMD_H_
