//
// Created by adam on 11.04.19.
//

#include <exception>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include "client.h"

Client::Client() {}

void Client::_createSocket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        throw std::runtime_error("Opening stream socket failed");
}

void Client::_connect(const char* serverName, unsigned port) {
    server.sin_family = AF_INET;
    hp = gethostbyname(serverName);
    if(hp == (struct hostent*) 0)
        throw std::runtime_error("Uknown server.");

    memcpy((char*) &server.sin_addr, hp->h_addr,(size_t) hp->h_length);
    server.sin_port = htons (port);

    if (connect(sock, (struct sockaddr*) &server, sizeof server) < 0)
        throw std::runtime_error("Connection failed.");
}


