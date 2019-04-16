// Copyright 2019 Kamil Mankowski

#include <iostream>

#include "Interpreter.h"

void Interpreter::interpretChar(char c, GlobalData *gdata) {
  if (inState == InputState::UntilNewLine && c == '\n') {
    proceedInput(gdata);
  } else {
    tmp += c;
    if (inState == InputState::GivenBytes && tmp.size() == bytesToRead) {
      proceedInput(gdata);
    }
  }
}

void Interpreter::proceedInput(GlobalData *gdata) {
  if (actionState == ActionState::SelectCommand) {
    std::string commName = tmp.substr(0, 12);

    if (commName.compare("HELLO_SERVER") == 0) {
      currentCommand = new HelloCmd(userid);
    } else if (commName.compare("SEND_MESSAGE") == 0) {
      currentCommand = new SendMessageCmd(userid);
    } else {
      tmp = "";  // TO DO: handle invalid command
      return;
    }

    bytesToRead = std::stoi(tmp.substr(12, 16));
    inState = InputState::GivenBytes;  // TO DO: check if bytesToRead > 0
    actionState = ActionState::PushToCommand;
  } else {
    auto retState = currentCommand->pushInput(tmp, &bytesToRead, gdata);
    setStates(retState);
  }
  tmp = "";
}

void Interpreter::setStates(Command::ReturnState rstate) {
  if (rstate == Command::CommandEnded) {
    delete currentCommand;
    inState = InputState::GivenBytes;
    bytesToRead = 16;
    actionState = ActionState::SelectCommand;
  } else if (rstate == Command::ReadLine) {
    inState = InputState::UntilNewLine;
    actionState = ActionState::PushToCommand;
  } else {
    inState = InputState::GivenBytes;
    actionState = ActionState::PushToCommand;
  }
}
