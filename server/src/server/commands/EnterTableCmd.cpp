// Copyright 2019 Kamil Mankowski

#include "EnterTableCmd.h"

const buffer_ptr EnterTableCmd::HEADER = helpers::to_buf("ENTER__TABLE");

void EnterTableCmd::pushInput(buffer_ptr input) {
  size_t interpreted_chars;
  int table_id;

  if (input->size() == 0 || current_user->isInTable()) {
    current_user->addMessageToQueue(Command::INVALID_COMMAND_PACKET);
    return;
  }

  table_id = std::stoi(helpers::to_str(input), &interpreted_chars);
  if (interpreted_chars != input->size()) {
    current_user->addMessageToQueue(Command::INVALID_COMMAND_PACKET);
    return;
  }

  if (!tables->isTable(table_id)) {
    current_user->addMessageToQueue(Command::INVALID_COMMAND_PACKET);
  } else {
    auto& table = tables->getTable(table_id);
    table.addPlayer(current_user);
  }
}
