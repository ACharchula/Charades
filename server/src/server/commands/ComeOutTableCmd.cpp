// Copyright 2019 Kamil Mankowski

#include "ComeOutTableCmd.h"

const buffer_ptr ComeOutTableCmd::HEADER = helpers::to_buf("COMEOUTTABLE");

void ComeOutTableCmd::pushInput(buffer_ptr input) {
  auto current_table = current_user->getTableId();
  auto& table = tables->getTable(current_table);
  table.removePlayer(current_user);
  current_user->setTableId(User::NO_TABLE);
}
