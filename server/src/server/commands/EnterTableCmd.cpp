// Copyright 2019 Kamil Mankowski

#include "EnterTableCmd.h"

const char EnterTableCmd::HEADER[] = "ENTER__TABLE";
const char EnterTableCmd::update_header[] = "UPDATECANVAS";

void EnterTableCmd::pushInput(std::string input, GlobalData *gdata) {
  gdata->addMessageToQueue(userid, update_header);
  auto canvasPtr = gdata->getTable().pointerToCanvas();
  gdata->addMessageToQueue(userid,
                           helpers::get_zero_width_size(canvasPtr->size()));
  gdata->addMessageToQueue(userid, canvasPtr);
}
