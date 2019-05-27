// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_LISTTABLESCMD_H_
#define SRC_SERVER_COMMANDS_LISTTABLESCMD_H_

#include "Command.h"

class ListTablesCmd : public Command {
 public:
  using Command::Command;
  void pushInput(buffer_ptr input) override;

  static const buffer_ptr HEADER;

 private:
  static const buffer_ptr SEE_TABLES;
};

#endif  // SRC_SERVER_COMMANDS_LISTTABLESCMD_H_
