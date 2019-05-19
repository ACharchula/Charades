// Copyright 2019 Kamil Mankowski

#include "Interpreter.h"

void Interpreter::interpretChar(char c) {
  tmp += c;
  while (tmp.size() == bytesToRead) {
    proceedInput();
    tmp = "";
  }
}

void Interpreter::proceedInput() {
  if (actionState == ActionState::SelectCommand) {
    if (tmp.compare(HelloCmd::HEADER) == 0) {
      currentCommand = new HelloCmd(current_user, tables, users);
    } else if (!current_user->isLogged()) {
      helpers::log("Not logged user try to execute non-hello command");
      throw std::exception();
    } else if (tmp.compare(SendMessageCmd::HEADER) == 0) {
      currentCommand = new SendMessageCmd(current_user, tables, users);
    } else if (tmp.compare(EnterTableCmd::HEADER) == 0) {
      currentCommand = new EnterTableCmd(current_user, tables, users);
    } else if (tmp.compare(SetCanvasCmd::HEADER) == 0) {
      currentCommand = new SetCanvasCmd(current_user, tables, users);
    } else {
      throw std::exception();
    }

    setLengthState();
  } else if (actionState == ActionState::ReadLength) {
    setPushState();
  } else {
    currentCommand->pushInput(tmp);
    delete currentCommand;
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
  bytesToRead = std::stoi(tmp, &interpreted_chars);
  if (interpreted_chars != currentCommand->lengthSize()) throw std::exception();
}

void Interpreter::setSelectCommandState() {
  actionState = ActionState::SelectCommand;
  bytesToRead = Command::HEADER_SIZE;
}

Interpreter::~Interpreter() { delete currentCommand; }
