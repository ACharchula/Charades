// Copyright 2019 Kamil Mankowski

#include "SetCanvasCmd.h"

const buffer_ptr SetCanvasCmd::HEADER = helpers::to_buf("SET___CANVAS");

void SetCanvasCmd::pushInput(buffer_ptr input) {
  auto table_id = current_user->getTableId();
  auto& table = tables->getTable(table_id);

  if (!table.isUserInTable(current_user)) return;

  table.setCanvas(input, current_user);
}
