// Copyright 2019 Kamil Mankowski

#include "CreateTableCmd.h"

const buffer_ptr CreateTableCmd::HEADER = helpers::to_buf("CREATE_TABLE");
const buffer_ptr CreateTableCmd::TABLE_CREATED =
    helpers::to_buf("TABLECREATED");

void CreateTableCmd::pushInput(buffer_ptr input) {
  int table_id = tables->createTable();
  auto data = std::to_string(table_id);
  helpers::log("Created table " + data, current_user->getId());

  current_user->addMessageToQueue(TABLE_CREATED);
  current_user->addMessageToQueue(helpers::get_zero_width_size(data.size()));
  current_user->addMessageToQueue(data);
}
