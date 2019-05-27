// Copyright 2019 Kamil Mankowski

#include "Tables.h"

int Tables::createTable() {
  tables.insert(std::make_pair(nextId, Table(&users, nextId)));
  return nextId++;
}

void Tables::proceedAll() {
  for (auto &table : tables) table.second.proceed();
}

std::vector<int> Tables::getTablesIds() {
  std::vector<int> ids;
  for (auto &table : tables) ids.push_back(table.first);
  return ids;
}
