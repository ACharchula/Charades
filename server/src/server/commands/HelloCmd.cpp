// Copyright 2019 Kamil Mankowski

#include "HelloCmd.h"

const buffer_ptr HelloCmd::HEADER = helpers::to_buf("HELLO_SERVER");
const buffer_ptr HelloCmd::output_packet = helpers::to_buf("WELCOME_USER0000");

void HelloCmd::pushInput(buffer_ptr input) {
  auto name = helpers::to_str(input);
  helpers::log("Welcome user: " + name);
  current_user->login();
  current_user->setUsername(name);
  current_user->addMessageToQueue(output_packet);
}
