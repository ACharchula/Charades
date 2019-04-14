//
// Created by adam on 11.04.19.
//

#ifndef CPP_CLIENT_CLIENT_H
#define CPP_CLIENT_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include "Message.h"

const size_t MAXMESSAGESIZE = 255;

class Client {
    int sock;
    struct sockaddr_in server;
    struct hostent* hp;

    void _createSocket();

    void _connect(const char* serverName, unsigned port);

    void _send(const char* message);

    std::pair<char*, ssize_t> _receive(size_t expectedDataSize); // first- header, second- body

    Message* _receiveMessage(size_t expectedDataSize);

public:
    Client();

    ~Client();

    void run(const char* serverName, unsigned port);

    void send(const char* message);

    std::pair<Message*, Message*> receive();
};


#endif //CPP_CLIENT_CLIENT_H
