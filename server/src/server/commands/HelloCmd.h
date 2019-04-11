// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_HELLOCMD_H_
#define SRC_SERVER_COMMANDS_HELLOCMD_H_

#include <string>

#include "Command.h"

class HelloCmd : public Command {
 public:
  explicit HelloCmd(int userid) : Command(userid) {}
  ReturnState pushInput(std::string input, int *outWaitBytes,
                        const GlobalData &gdata) override;

 private:
  enum State { Start, WaitForName } state = State::Start;
};

#endif  // SRC_SERVER_COMMANDS_HELLOCMD_H_
