// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_TABLE_H_
#define SRC_SERVER_TABLE_H_

#include <fstream>
#include <vector>

class Table {
 public:
  Table();
  std::vector<char>* pointerToCanvas() { return &canvas; }  // const?
 private:
  std::vector<char> canvas;
  void loadStartCanvas();
};

#endif  // SRC_SERVER_TABLE_H_
