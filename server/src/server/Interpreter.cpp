// Copyright 2019 Kamil Mankowski

#include "Interpreter.h"

const buffer_ptr Interpreter::PING_PACKET = helpers::to_buf("PING____PING0000");
const std::chrono::seconds Interpreter::INACTIVE_PING_SEC(3);

Interpreter::Interpreter(User *current_user, Users *users, Tables *tables)
    : current_user(current_user), users(users), tables(tables) {
  setSelectCommandState();
  tmp = std::make_shared<buffer_t>();
  setActive();
}

void Interpreter::interpretChar(char c) {
  tmp->push_back(c);
  while (tmp->size() == bytesToRead) {
    proceedInput();
    tmp = std::make_shared<buffer_t>();
  }
  setActive();
}

void Interpreter::setActive() {
  last_active = std::chrono::system_clock::now();
  waitForPong = false;
}

bool Interpreter::isActive() {
  auto timeDiff = std::chrono::system_clock::now() - last_active;
  auto secDiff = std::chrono::duration_cast<std::chrono::seconds>(timeDiff);

  if (secDiff > 2 * INACTIVE_PING_SEC) return false;

  if (secDiff > INACTIVE_PING_SEC && !waitForPong) {
    current_user->addMessageToQueue(PING_PACKET);
    waitForPong = true;
  }
  return true;
}

void Interpreter::proceedInput() {
  if (actionState == ActionState::SelectCommand) {
    if (equal(tmp, PongCmd::HEADER)) {
      currentCommand = std::make_unique<PongCmd>(current_user, tables, users);
    } else if (equal(tmp, HelloCmd::HEADER)) {
      currentCommand = std::make_unique<HelloCmd>(current_user, tables, users);
    } else if (!current_user->isLogged()) {
      helpers::log("Not logged user try to execute non-hello command");
      throw std::exception();
    } else if (equal(tmp, GetStatisticCmd::HEADER)) {
      currentCommand =
          std::make_unique<GetStatisticCmd>(current_user, tables, users);
    } else if (equal(tmp, CreateTableCmd::HEADER)) {
      currentCommand =
          std::make_unique<CreateTableCmd>(current_user, tables, users);
    } else if (equal(tmp, ListTablesCmd::HEADER)) {
      currentCommand =
          std::make_unique<ListTablesCmd>(current_user, tables, users);
    } else if (equal(tmp, EnterTableCmd::HEADER)) {
      currentCommand =
          std::make_unique<EnterTableCmd>(current_user, tables, users);
    } else if (!current_user->isInTable()) {
      helpers::log(
          "User is not in table, but try to execute table-specific command;");
      throw std::exception();
    } else if (equal(tmp, SendMessageCmd::HEADER)) {
      currentCommand =
          std::make_unique<SendMessageCmd>(current_user, tables, users);
    } else if (equal(tmp, SetCanvasCmd::HEADER)) {
      currentCommand =
          std::make_unique<SetCanvasCmd>(current_user, tables, users);
    } else if (equal(tmp, ComeOutTableCmd::HEADER)) {
      currentCommand =
          std::make_unique<ComeOutTableCmd>(current_user, tables, users);
    } else if (equal(tmp, GiveUpCmd::HEADER)) {
      currentCommand = std::make_unique<GiveUpCmd>(current_user, tables, users);
    } else {
      throw std::exception();
    }

    setLengthState();
  } else if (actionState == ActionState::ReadLength) {
    setPushState();
  } else {
    currentCommand->pushInput(tmp);
    currentCommand = nullptr;
    setSelectCommandState();
  }
}

void Interpreter::setLengthState() {
  actionState = ActionState::ReadLength;
  bytesToRead = currentCommand->lengthSize();
}

void Interpreter::setPushState() {
  size_t interpreted_chars;

  actionState = ActionState::PushToCommand;
  bytesToRead = std::stoi(helpers::to_str(tmp), &interpreted_chars);
  if (interpreted_chars != currentCommand->lengthSize()) throw std::exception();
}

void Interpreter::setSelectCommandState() {
  actionState = ActionState::SelectCommand;
  bytesToRead = Command::HEADER_SIZE;
}

void Interpreter::disconnect() {
  int table_id = current_user->getTableId();
  if (current_user->getTableId() != User::NO_TABLE)
    tables->getTable(table_id).removePlayer(current_user);
}
