// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMAND_H_
#define SRC_SERVER_COMMAND_H_

#include <string>

#include "../GlobalData.h"
#include "Helpers.h"

class Command {
 public:
  enum ReturnState { ReadLine, ReadBytes, CommandEnded };

  Command() {}
  explicit Command(int userid) : userid(userid) {}
  virtual ReturnState pushInput(std::string input, int *outWaitBytes,
                                GlobalData *gdata) {}

 protected:
  int userid = -1;
};

#endif  // SRC_SERVER_COMMAND_H_
