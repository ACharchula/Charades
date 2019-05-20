// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_TABLE_H_
#define SRC_SERVER_TABLE_H_

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "User.h"
#include "commands/Helpers.h"

class Table {
 public:
  Table() {}
  Table(Users *users, int id);

  // use User& or ptr instead of id
  void proceed();
  void checkClue(std::string propose, User *user);
  void addPlayer(User *user);
  void removePlayer(User *user) { players.erase(user); }
  void sendToAllExcept(std::string msg, User *user);
  void sendCurrentCanvas(User *user);
  void sendCurrentStatus(User *user);
  void setCanvas(std::string input, User *user);
  bool isUserInTable(User *user);

  int getId() { return id; }

 private:
  int id;
  bool canvasUpdated = false;

  std::vector<char> canvas;
  std::string clue = "kogut";

  std::set<User *> players;
  User *drawer;
  User *winner;

  enum State { WAITING, READY, ENDED } state = WAITING;

  Users *users;
  void loadStartCanvas();

  void setGameEnd(User *user);
  void sendToAllExcept(std::vector<char> *buff_ptr, User *user);
  void sendToAll(std::string msg) { sendToAllExcept(msg, nullptr); }
  void sendToAll(std::vector<char> *buff_ptr) {
    sendToAllExcept(buff_ptr, nullptr);
  }

  void sendUpdateCanvasIfNeeded();
  void proceedGameEndIfNeeded();
  void startGameIfNeeded();

  User *getRandomPlayer();

  std::string getRandomClue() { return "kurczak"; }  // make static

  static const char GAME_WAITING_PACKET[];
  static const char GAME_READY[];
  static const char GAME_ENDED[];
  static const char UPDATE_CANVAS[];
  static const char CLUE_CORRECT_PACKET[];
  static const char CLUE_INCORRECT_PACKET[];
  static const char YOU_ARE_DRAWER[];

  static const int CANVAS_LENGTH_SIZE = 8;
};

class Tables {
 public:
  explicit Tables(Users &users) : users(users) {}
  Table &getTable(int id) { return tables[id]; }
  int createTable() {
    tables.insert(std::make_pair(nextId, Table(&users, nextId)));
    return nextId++;
  }
  void proceedAll() {
    for (auto &table : tables) table.second.proceed();
  }
  // int removeTable(int id);
 private:
  Users &users;
  int nextId = 0;
  std::map<int, Table> tables;
};

#endif  // SRC_SERVER_TABLE_H_
