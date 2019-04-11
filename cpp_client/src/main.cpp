#include <iostream>
#include "client/client.h"

pthread_t sendThread, readThread;
Client* client;

void* send(void* arg) {
    char* message = new char[MAXMESSAGESIZE];
    for (;;) {
        std::cin >> message;
        client->send(message);
    }
}

void* read(void* arg) {
    const char* message;
    for (;;) {
        message = client->receive();
        std::cout << message;
        delete message;
    }
}

int main(int argc, char** argv) {

    if (argc < 3) {
        std::cerr << "Run ./exe <serverName> <port>";
        return -1;
    }

    client = new Client();
    client->run(argv[1], atoi(argv[2]));

    pthread_create(&sendThread, nullptr, &send, nullptr);
    pthread_create(&readThread, nullptr, &read, nullptr);

    pthread_join(sendThread, nullptr);
    pthread_join(readThread, nullptr);

    delete client;
    return 0;
}