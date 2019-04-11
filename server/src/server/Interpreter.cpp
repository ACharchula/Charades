// Copyright 2019 Kamil Mankowski

#include <iostream>

#include "Interpreter.h"

void Interpreter::interpretChar(char c, const GlobalData &gdata) {
  if (inState == InputState::UntilNewLine && c == '\n') {
    proceedInput(gdata);
  } else {
    tmp += c;
    if (inState == InputState::GivenBytes && tmp.size() == bytesToRead) {
      proceedInput(gdata);
    }
  }
}

void Interpreter::proceedInput(const GlobalData &gdata) {
  if (actionState == ActionState::SelectCommand) {
    if (tmp.compare("HELLO") == 0)
      currentCommand = new HelloCmd(userid);
    else {
      tmp = "";  // TODO: handle invalid command
      return;
    }
  }
  auto retState = currentCommand->pushInput(tmp, &bytesToRead, gdata);
  setStates(retState);
  tmp = "";
}

void Interpreter::setStates(Command::ReturnState rstate) {
  if (rstate == Command::CommandEnded) {
    delete currentCommand;
    inState = InputState::UntilNewLine;
    actionState = ActionState::SelectCommand;
  } else if (rstate == Command::ReadLine) {
    inState = InputState::UntilNewLine;
    actionState = ActionState::PushToCommand;
  } else {
    inState = InputState::GivenBytes;
    actionState = ActionState::PushToCommand;
  }
}
