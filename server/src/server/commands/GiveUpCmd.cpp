// Copyright 2019 Kamil Mankowski

#include "GiveUpCmd.h"

const buffer_ptr GiveUpCmd::HEADER = helpers::to_buf("GIVE_UP_GAME");

void GiveUpCmd::pushInput(buffer_ptr input) {
  auto table_id = current_user->getTableId();
  tables->getTable(table_id).tryGiveUp(current_user);
}
