// Copyright 2019 Kamil Mankowski

#include <iostream>
#include "server/Server.h"

int main(int argc, char **argv) {
  int port = 0;
  if (argc > 1) port = htons(atoi(argv[1]));
  Server srv(port);

  srv.prepare();
  std::cout << "Server runs on localhost:" << srv.getPort() << std::endl;
  srv.run();
}
