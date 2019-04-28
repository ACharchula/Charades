// Copyright 2019 Kamil Mankowski

#include "HelloCmd.h"

const char HelloCmd::HEADER[] = "HELLO_SERVER";
const char HelloCmd::output_packet[] = "WELCOME_USER0000";

void HelloCmd::pushInput(std::string input, GlobalData *gdata) {
  helpers::log("Welcome user: " + input);
  gdata->login(userid);
  gdata->setUsername(userid, input);
  gdata->addMessageToQueue(userid, output_packet);
}
