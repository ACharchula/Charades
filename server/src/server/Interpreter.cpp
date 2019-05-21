// Copyright 2019 Kamil Mankowski

#include "Interpreter.h"

Interpreter::Interpreter(User *current_user, Users *users, Tables *tables)
    : current_user(current_user), users(users), tables(tables) {
  setSelectCommandState();
  tmp = std::make_shared<buffer_t>();
}

void Interpreter::interpretChar(char c) {
  tmp->push_back(c);
  while (tmp->size() == bytesToRead) {
    proceedInput();
    tmp = std::make_shared<buffer_t>();
  }
}

void Interpreter::proceedInput() {
  if (actionState == ActionState::SelectCommand) {
    if (equal(tmp, HelloCmd::HEADER)) {
      currentCommand = std::make_unique<HelloCmd>(current_user, tables, users);
    } else if (!current_user->isLogged()) {
      helpers::log("Not logged user try to execute non-hello command");
      throw std::exception();
    } else if (equal(tmp, SendMessageCmd::HEADER)) {
      currentCommand =
          std::make_unique<SendMessageCmd>(current_user, tables, users);
    } else if (equal(tmp, EnterTableCmd::HEADER)) {
      currentCommand =
          std::make_unique<EnterTableCmd>(current_user, tables, users);
    } else if (equal(tmp, SetCanvasCmd::HEADER)) {
      currentCommand =
          std::make_unique<SetCanvasCmd>(current_user, tables, users);
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
