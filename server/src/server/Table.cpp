// Copyright 2019 Kamil Mankowski

#include "Table.h"

const buffer_ptr Table::GAME_WAITING_PACKET =
    helpers::to_buf("GAME_WAITING0000");
const buffer_ptr Table::GAME_READY = helpers::to_buf("GAME___READY");
const buffer_ptr Table::GAME_ENDED = helpers::to_buf("GAME___ENDED");
const buffer_ptr Table::GAME_ABORTED = helpers::to_buf("GAME_ABORTED");
const buffer_ptr Table::UPDATE_CANVAS = helpers::to_buf("UPDATECANVAS");
const buffer_ptr Table::YOU_ARE_DRAWER = helpers::to_buf("YOUAREDRAWER");
const buffer_ptr Table::CHAT_MESSAGE = helpers::to_buf("CHAT_MESSAGE");

const char Table::INITIAL_PICTURE_FILE[] = "data/start_canvas.png";
const char Table::WORDS_FILE[] = "data/words.txt";

std::vector<std::string> Table::WORDS;

Table::Table(Users* users, int id) : users(users), id(id) { loadStartCanvas(); }

void Table::loadStartCanvas() {
  std::ifstream start_canvas(INITIAL_PICTURE_FILE,
                             std::ifstream::binary | std::ifstream::ate);
  int size = start_canvas.tellg();
  start_canvas.seekg(0);
  std::istreambuf_iterator<char> start(start_canvas), end;

  canvas = std::make_shared<buffer_t>();
  canvas->reserve(size);
  canvas->assign(start, end);

  start_canvas.clear();
}

void Table::loadWords() {
  std::ifstream words_db(WORDS_FILE);
  std::string word;

  WORDS.clear();
  while (words_db && !words_db.eof()) {
    words_db >> word;
    if (word.size() > 0) {
      std::transform(word.begin(), word.end(), word.begin(), ::tolower);
      WORDS.push_back(word);
    }
  }

  helpers::log("Words DB loaded.");
  if (WORDS.size() == 0) {
    helpers::log("Words DB is empty, add 'kurczak'.");
    WORDS.push_back("kurczak");
  }
}

std::string Table::getRandomClue() {
  if (WORDS.size() == 0) loadWords();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, WORDS.size() - 1);

  int random = dist(gen);

  return WORDS[random];
}

void Table::checkClue(buffer_ptr propose, User* user) {
  if (user == drawer) {
    helpers::log("Drawer try to send message, ignored", user->getId());
    return;
  }

  auto propose_str = helpers::to_str(propose);
  std::transform(propose_str.begin(), propose_str.end(), propose_str.begin(),
                 ::tolower);
  sendUserMessage(propose, user);

  if (clue.compare(propose_str) == 0 && state == READY) {
    helpers::log("User won: " + user->getUsername(), user->getId());
    setGameEnd(user);
  }
}

void Table::sendUserMessage(buffer_ptr msg, User* author) {
  std::string user = author->getUsername() + "\n";
  int data_size = user.size() + msg->size();

  sendToAllExcept(CHAT_MESSAGE, author);
  sendToAllExcept(helpers::get_zero_width_size(data_size), author);
  sendToAllExcept(helpers::to_buf(user), author);
  sendToAllExcept(msg, author);
}

void Table::sendToAllExcept(buffer_ptr buff, User* except_user) {
  for (auto player : players) {
    if (player != except_user) {
      player->addMessageToQueue(buff);
    }
  }
}

void Table::setGameEnd(User* winner_ptr) {
  state = ENDED;
  winner = winner_ptr;
}

void Table::sendUpdateCanvasIfNeeded() {
  if (canvasUpdated) {
    sendToAllExcept(UPDATE_CANVAS, drawer);
    sendToAllExcept(
        helpers::get_zero_width_size(canvas->size(), CANVAS_LENGTH_SIZE),
        drawer);
    sendToAllExcept(canvas, drawer);

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
    helpers::log("Game ended.");
  }
}

void Table::proceedGameAbortIfNeeded() {
  if (state == ABORTED) {
    sendToAll(GAME_ABORTED);
    sendToAll(helpers::get_zero_width_size(clue.size()));
    sendToAll(clue);

    state = WAITING;
    helpers::log("Game aborted.");
  }
}

void Table::startGameIfNeeded() {
  if (state == WAITING && players.size() >= MINIMUM_PLAYERS) {
    loadStartCanvas();
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
  proceedGameAbortIfNeeded();
  proceedGameEndIfNeeded();
  startGameIfNeeded();
}

void Table::sendCurrentCanvas(User* user) {
  user->addMessageToQueue(UPDATE_CANVAS);
  user->addMessageToQueue(
      helpers::get_zero_width_size(canvas->size(), CANVAS_LENGTH_SIZE));
  user->addMessageToQueue(canvas);
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

void Table::setCanvas(buffer_ptr input, User* user) {
  if (user != drawer || state != READY) {
    helpers::log("Not the drawer try to draw or game is not READY, ignored.");
    return;
  }
  canvas = input;
  canvasUpdated = true;

  // THIS IS DEBUG ONLY AND WILL BE REMOVED
  std::ofstream debug_picture("data/debug_out.png", std::ofstream::binary);
  debug_picture.write(canvas->data(), canvas->size());
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

  sendCurrentStatus(user);
  sendCurrentCanvas(user);
  user->setTableId(getId());

  helpers::log("User entered to table", user->getId());
}

void Table::removePlayer(User* user) {
  helpers::log("User left the table", user->sock());
  players.erase(user);
  if (drawer == user) {
    helpers::log("Drawer left the table.");
    state = ABORTED;
  } else if (players.size() < MINIMUM_PLAYERS) {
    helpers::log("Number of players on table is less than minimum.");
    state = ABORTED;
  }
}

void Table::tryGiveUp(User* requester) {
  if (requester != drawer) {
    helpers::log("Try to GIVE UP, but requester is not the drawer, ignored.");
    return;
  }
  state = ABORTED;
  helpers::log("Drawer gave up the game.");
}
