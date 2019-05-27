// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_TABLES_H_
#define SRC_SERVER_TABLES_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Table.h"

class Tables {
 public:
  explicit Tables(Users &users) : users(users) {}
  Table &getTable(int id) { return tables.at(id); }
  int createTable();
  void proceedAll();
  std::vector<int> getTablesIds();
  bool isTable(int id) { return tables.count(id) == 1; }
  // int removeTable(int id);

 private:
  Users &users;
  int nextId = 0;
  std::map<int, Table> tables;
};

#endif  // SRC_SERVER_TABLES_H_
