// Copyright 2019 Kamil Mankowski

#include "EnterTableCmd.h"

const buffer_ptr EnterTableCmd::HEADER = helpers::to_buf("ENTER__TABLE");

void EnterTableCmd::pushInput(buffer_ptr input) {
  auto& table = tables->getTable(0);  // get from user

  table.addPlayer(current_user); // should do tho lines above
  table.sendCurrentStatus(current_user);
  table.sendCurrentCanvas(current_user);
  current_user->setTableId(table.getId());
}
