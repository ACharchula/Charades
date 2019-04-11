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

const size_t MAXMESSAGESIZE = 255;

class Client {
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;

    void _createSocket();
    void _connect(const char* serverName, unsigned port);
    void _send(const char* message);
    char* _receive();

public:
    Client();

    ~Client();

    void run(const char* serverName, unsigned port);

    void send(const char* message);

    char* receive();
};


#endif //CPP_CLIENT_CLIENT_H
