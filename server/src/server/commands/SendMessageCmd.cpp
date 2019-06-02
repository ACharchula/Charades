// Copyright 2019 Kamil Mankowski

#include "SendMessageCmd.h"

const buffer_ptr SendMessageCmd::HEADER = helpers::to_buf("SEND_MESSAGE");

void SendMessageCmd::pushInput(buffer_ptr input) {
  auto table_id = current_user->getTableId();
  auto& table = tables->getTable(table_id);

  if (!table.isUserInTable(current_user)) return;

  table.checkClue(input, current_user);
}
