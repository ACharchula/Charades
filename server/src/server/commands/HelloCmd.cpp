// Copyright 2019 Kamil Mankowski

#include <iostream>

#include "HelloCmd.h"

const char HelloCmd::HEADER[] = "HELLO_SERVER";
const char HelloCmd::output_packet[] = "WELCOME_USER0000";

void HelloCmd::pushInput(std::string input, GlobalData *gdata) {
  std::cout << "Welcome user: " << input << std::endl;
  gdata->addMessageToQueue(userid, output_packet);
}
