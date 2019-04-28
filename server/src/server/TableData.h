// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_TABLEDATA_H_
#define SRC_SERVER_TABLEDATA_H_

#include <algorithm>
#include <fstream>
#include <set>
#include <string>
#include <vector>

struct TableData {
 public:
  TableData();

  std::vector<char> canvas;
  std::string clue = "kogut";
  enum State { WAITING, READY, ENDED } state = WAITING;

  std::set<int> players;
  int drawer;
  int winner;

  bool canvasUpdated = false;

 private:
  void loadStartCanvas();
};

#endif  // SRC_SERVER_TABLEDATA_H_
