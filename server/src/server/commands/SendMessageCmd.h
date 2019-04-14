// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_SENDMESSAGECMD_H_
#define SRC_SERVER_COMMANDS_SENDMESSAGECMD_H_

#include <string>

#include "Command.h"

class SendMessageCmd : public Command {
 public:
  explicit SendMessageCmd(int userid) : Command(userid) {}
  ReturnState pushInput(std::string input, int *outWaitBytes,
                        GlobalData *gdata) override;

 private:
  enum State { Start, WaitForLength, WaitForMessage } state = State::Start;
};

#endif  // SRC_SERVER_COMMANDS_SENDMESSAGECMD_H_
