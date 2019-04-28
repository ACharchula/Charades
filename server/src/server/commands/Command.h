// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMAND_H_
#define SRC_SERVER_COMMAND_H_

#include <string>

#include "../GlobalData.h"
#include "../TableMgmt.h"
#include "Helpers.h"

class Command {
 public:
  Command() {}
  explicit Command(int userid) : userid(userid) {}
  virtual void pushInput(std::string input, GlobalData *gdata) {}

  static const int HEADER_SIZE = 12;
  static const int DATA_LENGTH_SIZE = 4;

 protected:
  int userid = -1;
};

#endif  // SRC_SERVER_COMMAND_H_
