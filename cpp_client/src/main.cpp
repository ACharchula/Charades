#include <iostream>
#include <pthread.h>
#include "client/client.h"

pthread_t sendThread, readThread;
Client* client;

void* send(void* arg){
    char* message = new char[MAXMESSAGESIZE];
    for(;;){
        std::cin >> message;
        client->send(message);
    }
}

void* read(void* arg){
    const char* message;
    for(;;){
        message = client->receive();
        std::cout << message;
    }
}

int main(int argc, char** argv) {

    if (argc < 3){
        std::cerr << "Run ./exe <serverName> <port>";
        return -1;
    }

    client = new Client();
    client->run("localhost", 44444);

    pthread_create(&sendThread, NULL, &send, NULL);
    pthread_create(&readThread, NULL, &read, NULL);

    pthread_join(sendThread, NULL);
    pthread_join(readThread, NULL);

    return 0;
}