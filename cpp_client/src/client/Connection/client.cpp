#include <utility>

//
// Created by adam on 11.04.19.
//

#include <exception>
#include <stdexcept>
#include <iostream>
#include <zconf.h>
#include <cstring>
#include "client.h"
#include "../Consts.h"

Client::Client(std::string userName) : userName(std::move(userName)) {}

Client::~Client() {
    if (sock >= 0)
        close(sock);
}

void Client::_createSocket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        throw std::runtime_error(ERROROPEN);
}

void Client::_connect(const char* serverName, uint16_t port) {
    server.sin_family = AF_INET;
    hp = gethostbyname(serverName);
    if (hp == (struct hostent*) 0)
        throw std::runtime_error(ERRORUKNOWNSERVER);

    memcpy((char*) &server.sin_addr, hp->h_addr, (size_t) hp->h_length);
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*) &server, sizeof server) < 0)
        throw std::runtime_error(ERRORCONNECT);
}

void Client::_send(const char* message, size_t messageSize) {
    if (write(sock, message, messageSize) <= 0)
        throw std::runtime_error(ERRORWRITING);
}

std::pair<char*, ssize_t> Client::_receive(size_t expectedDataSize) {
    char* buffer = new char[expectedDataSize + 1];

    ssize_t result;
    if ((result = read(sock, buffer, expectedDataSize)) <= 0)
        throw std::runtime_error(ERRORREADING);

    return std::make_pair(buffer, result);
}

std::unique_ptr<Message> Client::_receiveMessage(size_t expectedDataSize) {
    std::unique_ptr<Message> message(new Message(expectedDataSize));

    do {
        std::pair<char*, ssize_t> nextData = _receive(expectedDataSize);
        expectedDataSize -= nextData.second;
        message->append(nextData);
    } while (expectedDataSize);

    return std::move(message);
}

std::string Client::_getMessageSize(size_t size, std::string messageType) {
    std::string result;

    int i;
    if (messageType == SET || messageType == UPDATE)
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

std::pair<const char*, size_t> Client::_preparedMessage(std::string message, std::string messageType) {
    std::string result;
    result += messageType;
    result += _getMessageSize(message.size(), messageType);
    result += message;
    char* ret = new char[result.size()];
    result.copy(ret, result.size());
    return std::make_pair(ret, result.size());
}

void Client::run(const char* serverName, uint16_t port) {
    _createSocket();
    _connect(serverName, port);
    send(userName, HELLO);
}

void Client::send(std::string message, std::string messageType) {
    std::pair<const char*, size_t> messageToSend = _preparedMessage(std::move(message), std::move(messageType));
    _send(messageToSend.first, messageToSend.second);
}

std::pair<std::unique_ptr<Message>, std::unique_ptr<Message>> Client::receive() {
    auto header = _receiveMessage(HEADERSIZE);
    std::unique_ptr<Message> bodySize;

    if (header->equal(SET) || header->equal(UPDATE))
        bodySize = _receiveMessage(LONG);
    else
        bodySize = _receiveMessage(SHORT);

    std::unique_ptr<Message> body;
    if (bodySize->getSize() != 0)
        body = _receiveMessage(bodySize->getSize());

    return std::make_pair(std::move(header), std::move(body));
}