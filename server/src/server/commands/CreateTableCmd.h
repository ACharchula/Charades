// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_CREATETABLECMD_H_
#define SRC_SERVER_COMMANDS_CREATETABLECMD_H_

#include "Command.h"

class CreateTableCmd : public Command {
 public:
  using Command::Command;
  void pushInput(buffer_ptr input) override;

  static const buffer_ptr HEADER;

 private:
  static const buffer_ptr TABLE_CREATED;
};

#endif  // SRC_SERVER_COMMANDS_CREATETABLECMD_H_
