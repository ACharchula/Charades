// Copyright 2019 Kamil Mankowski

#include "HelloCmd.h"

const buffer_ptr HelloCmd::HEADER = helpers::to_buf("HELLO_SERVER");
const buffer_ptr HelloCmd::output_packet = helpers::to_buf("WELCOME_USER0000");

void HelloCmd::pushInput(buffer_ptr input) {
  auto name = helpers::to_str(input);

  if (current_user->isLogged()) {
    helpers::log("User is logged, can't relogin.");
    current_user->addMessageToQueue(Command::INVALID_COMMAND_PACKET);
  }

  if (users->loginUser(current_user, name)) {
    helpers::log("Welcome user: " + name);
    current_user->addMessageToQueue(output_packet);
  } else {
    helpers::log("Try to login already logged user:" + name,
                 current_user->sock());
    current_user->addMessageToQueue(Command::INVALID_COMMAND_PACKET);
  }
}
