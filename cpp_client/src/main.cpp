#include <iostream>
#include "client/client.h"

int main(int argc, char** argv) {

    if (argc < 3){
        std::cerr << "Run ./exe <serverName> <port>";
        return -1;
    }

    Client* client = new Client();
    client->run("localhost", 44444);

    std::string trash;
    std::cin >> trash;
    const char* result = client->receive();
    std::cout<<result;
    return 0;
}