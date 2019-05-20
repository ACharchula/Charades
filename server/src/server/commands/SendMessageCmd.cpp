// Copyright 2019 Kamil Mankowski

#include "SendMessageCmd.h"

const buffer_ptr SendMessageCmd::HEADER = helpers::to_buf("SEND_MESSAGE");
const buffer_ptr SendMessageCmd::output_header =
    helpers::to_buf("CHAT_MESSAGE");

void SendMessageCmd::pushInput(buffer_ptr input) {
  auto table = tables->getTable(0);

  if (!table.isUserInTable(current_user)) return;

  std::string user = current_user->getUsername() + "\n";
  int data_size = user.size() + input->size();

  table.sendToAllExcept(output_header, current_user);
  table.sendToAllExcept(helpers::get_zero_width_size(data_size), current_user);
  table.sendToAllExcept(helpers::to_buf(user), current_user);
  table.sendToAllExcept(input, current_user);

  table.checkClue(input, current_user);
}
