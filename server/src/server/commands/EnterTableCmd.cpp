// Copyright 2019 Kamil Mankowski

#include "EnterTableCmd.h"

const char EnterTableCmd::HEADER[] = "ENTER__TABLE";
const char EnterTableCmd::update_header[] = "UPDATECANVAS";

void EnterTableCmd::pushInput(std::string input, GlobalData *gdata) {
  TableMgmt tmgmt(gdata->getTable(), *gdata);

  tmgmt.addPlayer(userid);
  tmgmt.sendCurrentStatus(userid);
  tmgmt.sendCurrentCanvas(userid);
}
