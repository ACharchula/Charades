// Copyright 2019 Kamil Mankowski

#include "SendMessageCmd.h"

const char SendMessageCmd::HEADER[] = "SEND_MESSAGE";
const char SendMessageCmd::output_header[] = "CHAT_MESSAGE";

void SendMessageCmd::pushInput(std::string input) {
  auto table = tables->getTable(0);

  if (!table.isUserInTable(current_user)) return;

  std::string data = current_user->getUsername() + "\n" + input;
  std::string header =
      output_header + helpers::get_zero_width_size(data.size());

  table.sendToAllExcept(header + data, current_user); // td: move inside table
  table.checkClue(input, current_user);
}
