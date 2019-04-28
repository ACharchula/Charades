// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_TABLEMGMT_H_
#define SRC_SERVER_TABLEMGMT_H_

#include <algorithm>
#include <string>
#include <vector>

#include "GlobalData.h"
#include "TableData.h"
#include "commands/Helpers.h"

class TableMgmt {
 public:
  TableMgmt(TableData& tdata, GlobalData& gdata) : table(tdata), gdata(gdata) {}

  //   std::vector<char> *pointerToCanvas() { return &canvas; }  // const?
  void proceed();
  void checkClue(std::string propose, int userid);
  void addPlayer(int userid) { table.players.insert(userid); }
  void removePlayer(int userid) { table.players.erase(userid); }
  void sendToAllExcept(std::string msg, int userid);
  void sendCurrentCanvas(int userid);
  void sendCurrentStatus(int userid);
  void setCanvas(std::string input, int userid);

 private:
  TableData& table;
  GlobalData& gdata;

  void setGameEnd(int winner_id);
  void sendToAllExcept(std::vector<char>* buff_ptr, int userid);
  void sendToAll(std::string msg) { sendToAllExcept(msg, -1); }
  void sendToAll(std::vector<char>* buff_ptr) { sendToAllExcept(buff_ptr, -1); }

  void sendUpdateCanvasIfNeeded();
  void proceedGameEndIfNeeded();
  void startGameIfNeeded();

  int getRandomPlayer();
  std::string getRandomClue() { return "kurczak"; }

  static const char GAME_WAITING_PACKET[];
  static const char GAME_READY[];
  static const char GAME_ENDED[];
  static const char UPDATE_CANVAS[];
  static const char CLUE_CORRECT_PACKET[];
  static const char CLUE_INCORRECT_PACKET[];
  static const char YOU_ARE_DRAWER[];

  static const int CANVAS_LENGTH_SIZE = 8;
};

#endif  // SRC_SERVER_TABLEMGMT_H_
