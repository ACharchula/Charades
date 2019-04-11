//
// Created by adam on 11.04.19.
//

#ifndef CPP_CLIENT_CLIENT_H
#define CPP_CLIENT_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class Client {
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;

    void _createSocket();
    void _connect(const char* serverName, unsigned port);

public:
    Client();
};


#endif //CPP_CLIENT_CLIENT_H
