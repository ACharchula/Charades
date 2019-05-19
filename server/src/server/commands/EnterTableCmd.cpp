// Copyright 2019 Kamil Mankowski

#include "EnterTableCmd.h"

const char EnterTableCmd::HEADER[] = "ENTER__TABLE";
const char EnterTableCmd::update_header[] = "UPDATECANVAS";

void EnterTableCmd::pushInput(std::string input) {
  auto& table = tables->getTable(0);  // get from user
  // TableMgmt tmgmt(gdata->getTable(), *gdata);

  table.addPlayer(current_user);
  table.sendCurrentStatus(current_user);
  table.sendCurrentCanvas(current_user);
  current_user->setTableId(table.getId());
}
