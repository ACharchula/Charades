// Copyright 2019 Kamil Mankowski

#include "SendMessageCmd.h"

Command::ReturnState SendMessageCmd::pushInput(std::string input,
                                               int *outWaitBytes,
                                               GlobalData *gdata) {
  std::string data = "User: " + std::to_string(userid) + "\n" + input;
  std::string header =
      "CHAT_MESSAGE" + helpers::get_zero_width_size(data.size());
  for (auto id : gdata->getAllUsers()) {
    if (id != userid) {
      gdata->addMessageToQueue(id, header);
      gdata->addMessageToQueue(id, data);
    }
  }
  return ReturnState::CommandEnded;
}
