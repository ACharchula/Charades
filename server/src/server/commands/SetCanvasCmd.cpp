// Copyright 2019 Kamil Mankowski

#include "SetCanvasCmd.h"

const char SetCanvasCmd::HEADER[] = "SET___CANVAS";

void SetCanvasCmd::pushInput(std::string input) {
  auto table = tables->getTable(0);

  if (!table.isUserInTable(current_user)) return;

  table.setCanvas(input, current_user);
}
