// Copyright 2019 Kamil Mankowski

#include "SendMessageCmd.h"

const char SendMessageCmd::HEADER[] = "SEND_MESSAGE";
const char SendMessageCmd::output_header[] = "CHAT_MESSAGE";

void SendMessageCmd::pushInput(std::string input, GlobalData *gdata) {
  TableMgmt tmgmt(gdata->getTable(), *gdata);

  std::string data =
      "User: " + std::to_string(userid) + "\n" + input;  // TODO: username
  std::string header =
      output_header + helpers::get_zero_width_size(data.size());

  tmgmt.sendToAllExcept(header + data, userid);
  tmgmt.checkClue(input, userid);
}
