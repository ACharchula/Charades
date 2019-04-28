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
    if (tmp.compare(HelloCmd::HEADER) == 0) {
      currentCommand = new HelloCmd(userid);
    } else if (tmp.compare(SendMessageCmd::HEADER) == 0) {
      currentCommand = new SendMessageCmd(userid);
    } else if (tmp.compare(EnterTableCmd::HEADER) == 0) {
      currentCommand = new EnterTableCmd(userid);
    } else if (tmp.compare(SetCanvasCmd::HEADER) == 0) {
      currentCommand = new SetCanvasCmd(userid);
    } else {
      throw std::exception();
    }

    setLengthState();
  } else if (actionState == ActionState::ReadLength) {
    setPushState();
  } else {
    currentCommand->pushInput(tmp, gdata);
    delete currentCommand;
    currentCommand = nullptr;
    setSelectCommandState();
  }
}

void Interpreter::setLengthState() {
  actionState = ActionState::ReadLength;
  bytesToRead = currentCommand->lengthSize();
  helpers::log(std::to_string(bytesToRead));
}

void Interpreter::setPushState() {
  size_t interpreted_chars;

  actionState = ActionState::PushToCommand;
  bytesToRead = std::stoi(tmp, &interpreted_chars);
  if (interpreted_chars != currentCommand->lengthSize())
    throw std::exception();
}

void Interpreter::setSelectCommandState() {
  actionState = ActionState::SelectCommand;
  bytesToRead = Command::HEADER_SIZE;
}

Interpreter::~Interpreter() { delete currentCommand; }
