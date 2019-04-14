//
// Created by adam on 11.04.19.
//

#include <exception>
#include <stdexcept>
#include <iostream>
#include <zconf.h>
#include <cstring>
#include "client.h"

const size_t HEADERSIZE = 16;

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

std::pair<char*, ssize_t> Client::_receive(size_t expectedDataSize) {
    char* buffer = new char[expectedDataSize+1];

    ssize_t result;
    if ((result = read(sock, buffer, expectedDataSize)) < 0)
        throw std::runtime_error("Error reading from socket.");

    return std::make_pair(buffer, result);
}

Message* Client::_receiveMessage(size_t expectedDataSize) {
    Message* message = new Message(expectedDataSize);
    try {
        do{
            std::pair<char*, ssize_t> nextData = _receive(expectedDataSize);
            expectedDataSize -= nextData.second;
            message->append(nextData);
        } while(expectedDataSize);
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
    }

    return message;
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

std::pair<Message*, Message*> Client::receive() {
    Message* header = _receiveMessage(HEADERSIZE);
    std::cout <<header->getValue() <<std::endl;
    Message* body;

    return std::make_pair(nullptr, nullptr);
}

