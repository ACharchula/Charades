// Copyright 2019 Kamil Mankowski

#include "EnterTableCmd.h"

const buffer_ptr EnterTableCmd::HEADER = helpers::to_buf("ENTER__TABLE");

void EnterTableCmd::pushInput(buffer_ptr input) {
  auto& table = tables->getTable(0);  // get from user

  table.addPlayer(current_user);
}
