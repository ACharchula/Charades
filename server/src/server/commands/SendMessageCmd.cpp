// Copyright 2019 Kamil Mankowski

#include "SendMessageCmd.h"

const char SendMessageCmd::HEADER[] = "SEND_MESSAGE";
const char SendMessageCmd::output_header[] = "CHAT_MESSAGE";

void SendMessageCmd::pushInput(std::string input, GlobalData *gdata) {
  std::string data = "User: " + std::to_string(userid) + "\n" + input;
  std::string header =
      output_header + helpers::get_zero_width_size(data.size());
  for (auto id : gdata->getAllUsers()) {
    if (id != userid) {
      gdata->addMessageToQueue(id, header);
      gdata->addMessageToQueue(id, data);
    }
  }
}
