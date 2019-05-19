// Copyright 2019 Kamil Mankowski

#include "HelloCmd.h"

const char HelloCmd::HEADER[] = "HELLO_SERVER";
const char HelloCmd::output_packet[] = "WELCOME_USER0000";

void HelloCmd::pushInput(std::string input) {
  helpers::log("Welcome user: " + input);
  current_user->login();
  current_user->setUsername(input);
  current_user->addMessageToQueue(output_packet);
}
