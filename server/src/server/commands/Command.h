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
  virtual int lengthSize() { return data_length_size; }

  static const int HEADER_SIZE = 12;

 protected:
  int userid = -1;
  const int data_length_size = 4;
};

#endif  // SRC_SERVER_COMMAND_H_
