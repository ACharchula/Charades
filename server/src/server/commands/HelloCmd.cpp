// Copyright 2019 Kamil Mankowski

#include <iostream>

#include "HelloCmd.h"

Command::ReturnState HelloCmd::pushInput(std::string input, int *outWaitBytes,
                                         GlobalData *gdata) {
  // if (state == State::Start) {
  //   state = State::WaitForName;
  //   return ReturnState::ReadLine;
  // } else {
    std::cout << "Welcome user: " << input << std::endl;
    gdata->addMessageToQueue(userid, "WELCOME_USER0000");
    return ReturnState::CommandEnded;
  // }
}
