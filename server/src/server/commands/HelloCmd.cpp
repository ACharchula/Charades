// Copyright 2019 Kamil Mankowski

#include <iostream>

#include "HelloCmd.h"

Command::ReturnState HelloCmd::pushInput(std::string input, int *outWaitBytes,
                                           const GlobalData &gdata) {
  if (state == State::Start) {
    state = State::WaitForName;
    return ReturnState::ReadLine;
  } else {
    std::cout << input << std::endl;
    return ReturnState::CommandEnded;
  }
}
