// Copyright 2019 Kamil Mankowski

#include "SendMessageCmd.h"

const char SendMessageCmd::HEADER[] = "SEND_MESSAGE";
const char SendMessageCmd::output_header[] = "CHAT_MESSAGE";

void SendMessageCmd::pushInput(std::string input, GlobalData *gdata) {
  TableMgmt tmgmt(gdata->getTable(), *gdata);

  if (!tmgmt.isUserInTable(userid)) return;

  std::string data = gdata->getUsername(userid) + "\n" + input;
  std::string header =
      output_header + helpers::get_zero_width_size(data.size());

  tmgmt.sendToAllExcept(header + data, userid);
  tmgmt.checkClue(input, userid);
}
