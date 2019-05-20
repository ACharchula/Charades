// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_SENDMESSAGECMD_H_
#define SRC_SERVER_COMMANDS_SENDMESSAGECMD_H_

#include <string>

#include "Command.h"

class SendMessageCmd : public Command {
 public:
  using Command::Command;
  void pushInput(buffer_ptr input) override;

  static const buffer_ptr HEADER;
};

#endif  // SRC_SERVER_COMMANDS_SENDMESSAGECMD_H_
