//
// Created by adam on 11.04.19.
//

#include <exception>
#include <stdexcept>
#include <iostream>
#include <zconf.h>
#include <cstring>
#include "client.h"


Client::Client() {}

Client::~Client() {
    if (sock >= 0)
        close(sock);
}

void Client::_createSocket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        throw std::runtime_error("Opening stream socket failed");
}

void Client::_connect(const char* serverName, unsigned port) {
    server.sin_family = AF_INET;
    hp = gethostbyname(serverName);
    if (hp == (struct hostent*) 0)
        throw std::runtime_error("Uknown server.");

    memcpy((char*) &server.sin_addr, hp->h_addr, (size_t) hp->h_length);
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*) &server, sizeof server) < 0)
        throw std::runtime_error("Connection failed.");
}

void Client::_send(const char* message) {
    if (write(sock, message, strlen(message)) < 0)
        throw std::runtime_error("Error writing to socket.");
}

char* Client::_receive() {
    char* buffer = new char[MAXMESSAGESIZE];

    if (read(sock, buffer, MAXMESSAGESIZE) < 0)
        throw std::runtime_error("Error reading from socket.");
    return buffer;
}

void Client::run(const char* serverName, unsigned port) {
    try {
        _createSocket();
        _connect(serverName, port);
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
    }
}

void Client::send(const char* message) {
    try {
        _send(message);
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
    }
}

char* Client::receive() {
    char* message = nullptr;
    try {
        message = _receive();
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
    }
    return message;
}
