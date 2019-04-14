// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMAND_H_
#define SRC_SERVER_COMMAND_H_

#include <iostream>

// TODO(kmankow):
// One Command object on one TCP session, so it should have information
// about user in this session and current state in this session.
// On addChar this should parse input from user and have methods to
// make action for any protocol commands.
// Send to user: like ref to one messege queue checked by server? or by chair?
// to design in future

class Command {
 public:
  Command() {}
  void addChar(char newChar);
};

#endif  // SRC_SERVER_COMMAND_H_
