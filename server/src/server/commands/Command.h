// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMAND_H_
#define SRC_SERVER_COMMAND_H_

#include <string>

#include "../Table.h"
#include "../User.h"
#include "Helpers.h"

class Command {
 public:
  Command() {}
  explicit Command(User *current_user, Tables *tables, Users *users)
      : current_user(current_user), tables(tables) {}
  virtual void pushInput(std::string input) {}
  virtual int lengthSize() { return data_length_size; }

  static const int HEADER_SIZE = 12;

 protected:
  User *current_user = nullptr;
  const int data_length_size = 4;
  Tables *tables;
  Users *users;
};

#endif  // SRC_SERVER_COMMAND_H_
