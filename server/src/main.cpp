// Copyright 2019 Kamil Mankowski

#include <iostream>
#include "server/Server.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int PORT = 44444;
Server srv(PORT);

void handler(int s) {
  srv.stop();
}

int main(int argc, char **argv) {
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);

  srv.prepare();
  std::cout << "Server runs on localhost:" << srv.getPort() << std::endl;
  srv.run();
}
