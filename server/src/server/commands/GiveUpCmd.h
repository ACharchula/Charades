// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_GIVEUPCMD_H_
#define SRC_SERVER_COMMANDS_GIVEUPCMD_H_

#include "Command.h"

class GiveUpCmd : public Command {
 public:
  using Command::Command;
  void pushInput(buffer_ptr input) override;

  static const buffer_ptr HEADER;
};

#endif  // SRC_SERVER_COMMANDS_GIVEUPCMD_H_
