// Copyright 2019 Kamil Mankowski

#include "Interpreter.h"

void Interpreter::interpretChar(char c, GlobalData *gdata) {
  tmp += c;
  while (tmp.size() == bytesToRead) {
    proceedInput(gdata);
    tmp = "";
  }
}

void Interpreter::proceedInput(GlobalData *gdata) {
  if (actionState == ActionState::SelectCommand) {
    std::string commName = tmp.substr(0, Command::HEADER_SIZE);

    if (commName.compare(HelloCmd::HEADER) == 0) {
      currentCommand = new HelloCmd(userid);
    } else if (commName.compare(SendMessageCmd::HEADER) == 0) {
      currentCommand = new SendMessageCmd(userid);
    } else {
      throw std::exception();
    }

    setPushState();

  } else {
    currentCommand->pushInput(tmp, gdata);
    delete currentCommand;
    currentCommand = nullptr;
    setSelectCommandState();
  }
}

void Interpreter::setPushState() {
  size_t interpreted_chars;

  actionState = ActionState::PushToCommand;
  bytesToRead = std::stoi(tmp.substr(12, 16), &interpreted_chars);
  if (interpreted_chars != Command::DATA_LENGTH_SIZE) throw std::exception();
}

void Interpreter::setSelectCommandState() {
  actionState = ActionState::SelectCommand;
  bytesToRead = Command::HEADER_SIZE + Command::DATA_LENGTH_SIZE;
}

Interpreter::~Interpreter() { delete currentCommand; }
