// Copyright 2019 Kamil Mankowski

#include "SendMessageCmd.h"

Command::ReturnState SendMessageCmd::pushInput(std::string input,
                                               int *outWaitBytes,
                                               GlobalData *gdata) {
  if (state == State::Start) {
    state = State::WaitForLength;
    return ReturnState::ReadLine;
  } else if (state == State::WaitForLength) {
    *outWaitBytes = std::stoi(input);
    state = State::WaitForMessage;
    return ReturnState::ReadBytes;
  } else {
    for (auto id : gdata->getAllUsers()) {
      if (id != userid) {
        gdata->addMessageToQueue(id, "CHAT_MESSAGE\n");
        gdata->addMessageToQueue(id, "User: " + std::to_string(userid) + "\n");
        gdata->addMessageToQueue(id, std::to_string(input.size()) + "\n");
        gdata->addMessageToQueue(id, input + "\n");
      }
    }
    return ReturnState::CommandEnded;
  }
}
