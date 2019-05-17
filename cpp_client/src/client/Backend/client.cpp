//
// Created by adam on 11.04.19.
//

#include <exception>
#include <stdexcept>
#include <iostream>
#include <zconf.h>
#include <cstring>
#include "client.h"

Client::Client(const std::string userName) : userName(userName) {}

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

void Client::_send(const char* message, size_t messageSize) {
    if (write(sock, message, messageSize) < 0)
        throw std::runtime_error("Error writing to socket.");
}

std::pair<char*, ssize_t> Client::_receive(size_t expectedDataSize) {
    char* buffer = new char[expectedDataSize + 1];

    ssize_t result;
    if ((result = read(sock, buffer, expectedDataSize)) < 0)
        throw std::runtime_error("Error reading from socket.");

    return std::make_pair(buffer, result);
}

Message* Client::_receiveMessage(size_t expectedDataSize) {
    Message* message = new Message(expectedDataSize);
    try {
        do {
            std::pair<char*, ssize_t> nextData = _receive(expectedDataSize);
            expectedDataSize -= nextData.second;
            message->append(nextData);
        } while (expectedDataSize);
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
    }

    return message;
}

std::string Client::_getMessageSize(size_t size, const std::string messageType) {
    std::string result;

    int i;
    if(messageType == SET || messageType == UPDATE)
      i = 1000 * 1000 * 10;
    else
      i = 1000;
    for (; i > 0; i /= 10) {
        result += std::to_string((size / i));
        if (size / i > 0)
            size = size % i;
    }
    return result;
}

std::pair<const char*, size_t> Client::_preparedMessage(const std::string message, const std::string messageType) {
    std::string result;
    result += messageType;
    result += _getMessageSize(message.size(), messageType);
    result += message;
    char* ret = new char[result.size()];
    result.copy(ret, result.size());
    return std::make_pair(ret, result.size());
}

void Client::run(const char* serverName, unsigned port) {
    try {
        _createSocket();
        _connect(serverName, port);
        send(userName, HELLO);
        send("", ENTER);
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
    }
}

void Client::send(const std::string message, const std::string messageType) {
    try {
        std::pair<const char*, size_t> messageToSend = _preparedMessage(message, messageType);
        _send(messageToSend.first, messageToSend.second);
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
    }
}

std::pair<Message*, Message*> Client::receive() {
    Message* header = _receiveMessage(HEADERSIZE);
    Message* bodySize;

    if(header->equal(SET) || header->equal(UPDATE))
        bodySize = _receiveMessage(LONG);
    else
        bodySize = _receiveMessage(SHORT);
    Message* body = nullptr;
    if (bodySize->getSize() != 0)
        body = _receiveMessage(bodySize->getSize());

    return std::make_pair(header, body);
}

