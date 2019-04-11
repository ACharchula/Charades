#include <iostream>
#include "client/client.h"

int main(int argc, char** argv) {

    if (argc < 3){
        std::cerr << "Run ./exe <serverName> <port>";
        return -1;
    }

    Client* client = new Client();


    return 0;
}