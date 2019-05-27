// Copyright 2019 Kamil Mankowski

#include "ListTablesCmd.h"

const buffer_ptr ListTablesCmd::HEADER = helpers::to_buf("LIST__TABLES");
const buffer_ptr ListTablesCmd::SEE_TABLES = helpers::to_buf("SEE___TABLES");

void ListTablesCmd::pushInput(buffer_ptr input) {
  std::string data = "";
  for (int id : tables->getTablesIds()) data += std::to_string(id) + "\n";

  current_user->addMessageToQueue(SEE_TABLES);
  current_user->addMessageToQueue(helpers::get_zero_width_size(data.size()));
  current_user->addMessageToQueue(data);
}
