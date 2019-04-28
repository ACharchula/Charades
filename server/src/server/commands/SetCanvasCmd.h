// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_SETCANVASCMD_H_
#define SRC_SERVER_COMMANDS_SETCANVASCMD_H_

#include <string>

#include "Command.h"

class SetCanvasCmd : public Command {
 public:
  explicit SetCanvasCmd(int userid) : Command(userid) {}
  void pushInput(std::string input, GlobalData *gdata) override;

  static const char HEADER[];
};

#endif  // SRC_SERVER_COMMANDS_SETCANVASCMD_H_
