// Copyright 2019 Kamil Mankowski

#include "SetCanvasCmd.h"

const char SetCanvasCmd::HEADER[] = "SET___CANVAS";

void SetCanvasCmd::pushInput(std::string input, GlobalData *gdata) {
  TableMgmt tmgmt(gdata->getTable(), *gdata);
  tmgmt.setCanvas(input, userid);
}
