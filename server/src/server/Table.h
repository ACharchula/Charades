// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_TABLE_H_
#define SRC_SERVER_TABLE_H_

#include <fstream>
#include <map>
#include <random>
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

  void proceed();
  void checkClue(buffer_ptr propose, User *user);
  void addPlayer(User *user);
  void removePlayer(User *user);
  void setCanvas(buffer_ptr input, User *user);
  void tryGiveUp(User *requester);
  bool isUserInTable(User *user);

  int getId() { return id; }

 private:
  int id;
  bool canvasUpdated = false;

  buffer_ptr canvas;
  std::string clue = "kogut";

  std::set<User *> players;
  User *drawer;
  User *winner;

  enum State { WAITING, READY, ABORTED, ENDED } state = WAITING;

  Users *users;
  void loadStartCanvas();
  void loadWords();

  void setGameEnd(User *user);
  void sendToAllExcept(buffer_ptr buff_ptr, User *user);
  void sendToAll(std::string msg) {
    sendToAllExcept(helpers::to_buf(msg), nullptr);
  }
  void sendToAll(buffer_ptr buff) { sendToAllExcept(buff, nullptr); }
  void sendCurrentCanvas(User *user);
  void sendCurrentStatus(User *user);
  void sendUserMessage(buffer_ptr msg, User *author);

  void sendUpdateCanvasIfNeeded();
  void proceedGameEndIfNeeded();
  void startGameIfNeeded();
  void proceedGameAbortIfNeeded();

  User *getRandomPlayer();
  std::string getRandomClue();
  void log(std::string msg, int user = -1);

  static std::vector<std::string> WORDS;

  static const buffer_ptr GAME_WAITING_PACKET;
  static const buffer_ptr GAME_READY;
  static const buffer_ptr GAME_ENDED;
  static const buffer_ptr GAME_ABORTED;
  static const buffer_ptr UPDATE_CANVAS;
  static const buffer_ptr YOU_ARE_DRAWER;
  static const buffer_ptr CHAT_MESSAGE;

  static const int CANVAS_LENGTH_SIZE = 8;
  static const int MINIMUM_PLAYERS = 2;

  static const char INITIAL_PICTURE_FILE[];
  static const char WORDS_FILE[];
};

#endif  // SRC_SERVER_TABLE_H_
