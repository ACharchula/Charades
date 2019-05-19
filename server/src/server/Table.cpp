// Copyright 2019 Kamil Mankowski

#include "Table.h"

const char Table::GAME_WAITING_PACKET[] = "GAME_WAITING0000";
const char Table::GAME_READY[] = "GAME___READY";
const char Table::GAME_ENDED[] = "GAME___ENDED";
const char Table::UPDATE_CANVAS[] = "UPDATECANVAS";
const char Table::CLUE_CORRECT_PACKET[] = "CLUE_CORRECT0000";
const char Table::CLUE_INCORRECT_PACKET[] = "CLUEINCORECT0000";
const char Table::YOU_ARE_DRAWER[] = "YOUAREDRAWER";

Table::Table(Users* users, int id) : users(users), id(id) { loadStartCanvas(); }

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

  start_canvas.clear();
}

void Table::checkClue(std::string propose, User* user) {
  std::transform(propose.begin(), propose.end(), propose.begin(), ::tolower);
  if (user == drawer) {
    helpers::log("Drawer try to send message, ignored", user->getId());
  } else if (clue.compare(propose) != 0 || state == ENDED) {
    user->addMessageToQueue(CLUE_INCORRECT_PACKET);
  } else {
    helpers::log("User won: " + user->getUsername(), user->getId());
    user->addMessageToQueue(CLUE_CORRECT_PACKET);
    setGameEnd(user);
  }
}

void Table::sendToAllExcept(std::string msg, User* except_user) {
  for (auto player : players) {
    if (player != except_user) {
      player->addMessageToQueue(msg);
    }
  }
}

void Table::sendToAllExcept(std::vector<char>* buff_ptr, User* except_user) {
  for (auto player : players) {
    if (player != except_user) {
      player->addMessageToQueue(buff_ptr);
    }
  }
}

void Table::setGameEnd(User* winner_id) {
  state = ENDED;
  winner = winner_id;
}

void Table::sendUpdateCanvasIfNeeded() {
  if (canvasUpdated) {
    sendToAllExcept(UPDATE_CANVAS, drawer);
    sendToAllExcept(
        helpers::get_zero_width_size(canvas.size(), CANVAS_LENGTH_SIZE),
        drawer);
    sendToAllExcept(&canvas, drawer);

    canvasUpdated = false;
  }
}

void Table::proceedGameEndIfNeeded() {
  if (state == ENDED) {
    std::string data = winner->getUsername() + "\n" + clue;

    sendToAll(GAME_ENDED);
    sendToAll(helpers::get_zero_width_size(data.size()));
    sendToAll(data);

    state = WAITING;
  }
}

void Table::startGameIfNeeded() {
  if (state == WAITING && players.size() > 1) {
    clue = getRandomClue();
    drawer = getRandomPlayer();
    state = READY;

    sendToAll(GAME_READY);
    sendToAll(helpers::get_zero_width_size(drawer->getUsername().size()));
    sendToAll(drawer->getUsername());

    drawer->addMessageToQueue(YOU_ARE_DRAWER);
    drawer->addMessageToQueue(helpers::get_zero_width_size(clue.size()));
    drawer->addMessageToQueue(clue);

    helpers::log("Game started, clue: " + clue);
    helpers::log("Drawer is " + drawer->getUsername(), drawer->getId());
  }
}

void Table::proceed() {
  sendUpdateCanvasIfNeeded();
  proceedGameEndIfNeeded();
  startGameIfNeeded();
}

void Table::sendCurrentCanvas(User* user) {
  user->addMessageToQueue(UPDATE_CANVAS);
  user->addMessageToQueue(
      helpers::get_zero_width_size(canvas.size(), CANVAS_LENGTH_SIZE));
  user->addMessageToQueue(&canvas);
}

void Table::sendCurrentStatus(User* user) {
  if (state == READY) {
    user->addMessageToQueue(GAME_READY);
    user->addMessageToQueue(
        helpers::get_zero_width_size(drawer->getUsername().size()));
    user->addMessageToQueue(drawer->getUsername());
  } else {
    user->addMessageToQueue(GAME_WAITING_PACKET);
  }
}

User* Table::getRandomPlayer() {
  int move = std::rand() % players.size();
  auto it = players.begin();
  std::advance(it, move);
  return *it;
}

void Table::setCanvas(std::string input, User* user) {
  if (user != drawer) {
    helpers::log("Not a drawer try to draw");
    return;  // TODO: exception or send error packet
  }
  canvas.reserve(input.size());
  canvas.assign(input.begin(), input.end());
  canvasUpdated = true;

  // THIS IS DEBUG ONLY AND WILL BE REMOVED
  std::ofstream debug_picture("data/debug_out.png", std::ofstream::binary);
  debug_picture.write(canvas.data(), canvas.size());
  debug_picture.close();
}

bool Table::isUserInTable(User* user) {
  auto find_player = players.find(user);
  if (find_player == players.end())
    helpers::log("Player not on table", user->getId());
  return !(find_player == players.end());
}

void Table::addPlayer(User* user) {
  players.insert(user);
  helpers::log("User entered to table", user->getId());
}
