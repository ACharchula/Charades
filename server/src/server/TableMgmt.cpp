// Copyright 2019 Kamil Mankowski

#include "TableMgmt.h"

const char TableMgmt::GAME_WAITING_PACKET[] = "GAME_WAITING0000";
const char TableMgmt::GAME_READY[] = "GAME___READY";
const char TableMgmt::GAME_ENDED[] = "GAME___ENDED";
const char TableMgmt::UPDATE_CANVAS[] = "UPDATECANVAS";
const char TableMgmt::CLUE_CORRECT_PACKET[] = "CLUE_CORRECT0000";
const char TableMgmt::CLUE_INCORRECT_PACKET[] = "CLUEINCORECT0000";
const char TableMgmt::YOU_ARE_DRAWER[] = "YOUAREDRAWER";

void TableMgmt::checkClue(std::string propose, int userid) {
  std::transform(propose.begin(), propose.end(), propose.begin(), ::tolower);
  if (userid == table.drawer) {
    helpers::log("Drawer try to send message, ignored", userid);
  } else if (table.clue.compare(propose) != 0 || table.state == table.ENDED) {
    gdata.addMessageToQueue(userid, CLUE_INCORRECT_PACKET);
  } else {
    helpers::log("User won: " + gdata.getUsername(userid), userid);
    gdata.addMessageToQueue(userid, CLUE_CORRECT_PACKET);
    setGameEnd(userid);
  }
}

void TableMgmt::sendToAllExcept(std::string msg, int except_user) {
  for (auto id : table.players) {
    if (id != except_user) {
      gdata.addMessageToQueue(id, msg);
    }
  }
}

void TableMgmt::sendToAllExcept(std::vector<char>* buff_ptr, int except_user) {
  for (auto id : table.players) {
    if (id != except_user) {
      gdata.addMessageToQueue(id, buff_ptr);
    }
  }
}

void TableMgmt::setGameEnd(int winner_id) {
  table.state = table.ENDED;
  table.winner = winner_id;
}

void TableMgmt::sendUpdateCanvasIfNeeded() {
  if (table.canvasUpdated) {
    sendToAllExcept(UPDATE_CANVAS, table.drawer);
    sendToAllExcept(
        helpers::get_zero_width_size(table.canvas.size(), CANVAS_LENGTH_SIZE),
        table.drawer);
    sendToAllExcept(&table.canvas, table.drawer);

    table.canvasUpdated = false;
  }
}

void TableMgmt::proceedGameEndIfNeeded() {
  if (table.state == table.ENDED) {
    std::string data = gdata.getUsername(table.winner) + "\n" + table.clue;

    sendToAll(GAME_ENDED);
    sendToAll(helpers::get_zero_width_size(data.size()));
    sendToAll(data);

    table.state = table.WAITING;
  }
}

void TableMgmt::startGameIfNeeded() {
  if (table.state == table.WAITING && table.players.size() > 1) {
    table.clue = getRandomClue();
    table.drawer = getRandomPlayer();
    table.state = table.READY;

    std::string drawer_name = gdata.getUsername(table.drawer);
    sendToAll(GAME_READY);
    sendToAll(helpers::get_zero_width_size(drawer_name.size()));
    sendToAll(drawer_name);

    gdata.addMessageToQueue(table.drawer, YOU_ARE_DRAWER);
    gdata.addMessageToQueue(table.drawer,
                            helpers::get_zero_width_size(table.clue.size()));
    gdata.addMessageToQueue(table.drawer, table.clue);

    helpers::log("Game started, clue: " + table.clue);
    helpers::log("Drawer is " + gdata.getUsername(table.drawer), table.drawer);
  }
}

void TableMgmt::proceed() {
  sendUpdateCanvasIfNeeded();
  proceedGameEndIfNeeded();
  startGameIfNeeded();
}

void TableMgmt::sendCurrentCanvas(int userid) {
  gdata.addMessageToQueue(userid, UPDATE_CANVAS);
  gdata.addMessageToQueue(userid, helpers::get_zero_width_size(
                                      table.canvas.size(), CANVAS_LENGTH_SIZE));
  gdata.addMessageToQueue(userid, &table.canvas);
}

void TableMgmt::sendCurrentStatus(int userid) {
  if (table.state == table.READY) {
    gdata.addMessageToQueue(userid, GAME_READY);
    std::string drawe_name = gdata.getUsername(table.drawer);
    gdata.addMessageToQueue(userid,
                            helpers::get_zero_width_size(drawe_name.size()));
    gdata.addMessageToQueue(userid, drawe_name);
  } else {
    gdata.addMessageToQueue(userid, GAME_WAITING_PACKET);
  }
}

int TableMgmt::getRandomPlayer() {
  int move = std::rand() % table.players.size();
  auto it = table.players.begin();
  std::advance(it, move);
  return *it;
}

void TableMgmt::setCanvas(std::string input, int userid) {
  if (userid != table.drawer) {
    helpers::log("Not a drawer try to draw");
    return;  // TODO: exception or send error packet
  }
  table.canvas.reserve(input.size());
  table.canvas.assign(input.begin(), input.end());
  table.canvasUpdated = true;

  // THIS IS DEBUG ONLY AND WILL BE REMOVED
  std::ofstream debug_picture("data/debug_out.png", std::ofstream::binary);
  debug_picture.write(table.canvas.data(), table.canvas.size());
  debug_picture.close();
}

bool TableMgmt::isUserInTable(int userid) {
  auto find_player = table.players.find(userid);
  if (find_player == table.players.end())
    helpers::log("Player not on table", userid);
  return !(find_player == table.players.end());
}

void TableMgmt::addPlayer(int userid) {
  table.players.insert(userid);
  helpers::log("User entered to table", userid);
}
