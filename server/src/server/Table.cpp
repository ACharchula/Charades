// Copyright 2019 Kamil Mankowski

#include "Table.h"

Table::Table() { loadStartCanvas(); }

void Table::loadStartCanvas() {
  std::string filename =
      "data/start_canvas.png";  // TODO: this have to be configured!
  std::ifstream start_canvas(filename,
                             std::ifstream::binary | std::ifstream::ate);
  int size = start_canvas.tellg();
  start_canvas.seekg(0);
  std::istreambuf_iterator<char> start(start_canvas), end;

  canvas.reserve(size);
  canvas.assign(start, end);
}
