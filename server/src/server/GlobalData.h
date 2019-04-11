// Copyright 2019 Kamil Mankowski
// Store information accessible by all

#ifndef SRC_SERVER_GLOBALDATA_H_
#define SRC_SERVER_GLOBALDATA_H_

#include <map>

#include "Interpreter.h"
#include "Server.h"

class GlobalData {
 public:
  // send to users
  // get all users ids
 private:
  std::map<int, Interpreter> interpreters;

  friend class Server;
};

#endif  // SRC_SERVER_GLOBALDATA_H_
