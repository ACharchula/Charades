// Copyright 2019 Kamil Mankowski

#include <iostream>
#include "server/Server.h"

int main(int argc, char **argv) {
    Server srv;

    srv.prepare();
    std::cout << "Server run on localhost:" << srv.getPort() << std::endl;
    srv.run();
}
