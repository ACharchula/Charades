//
// Created by adam on 11.04.19.
//

#include <exception>
#include <stdexcept>
#include <iostream>
#include <zconf.h>
#include <cstring>
#include "client.h"

Client::Client(const char* userName) : userName (userName){}

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

Message* Client::_receiveMessage(size_t expectedDataSize, Message::Type type) {
    Message* message = new Message(expectedDataSize, type);
    try {
        do{
            std::pair<char*, ssize_t> nextData = _receive(expectedDataSize);
            expectedDataSize -= nextData.second;
            message->append(nextData);
        } while(expectedDataSize);
        message->endMessage();
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
    }

    return message;
}

std::string Client::_getMessageSize(size_t size) {
    std::string result;

    for(int i = 1000 ; i > 0; i /= 10){
        result += std::to_string((size/i));
        if(size/i > 0)
            size = size % i;
    }

    return result;
}

const char* Client::_preparedMessage(const std::string message, const std::string messageType){
    std::string result;
    result += messageType;
    result += _getMessageSize(message.size());
    result += message;
    result += '\0';
    char* ret = new char[result.size()];
    result.copy(ret, result.size());

    return ret;
}

void Client::run(const char* serverName, unsigned port) {
    try {
        _createSocket();
        _connect(serverName, port);
        send(userName, HELLO);
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
    }
}

void Client::send(const std::string message, const std::string messageType) {
    try {
        const char* messageToSend = _preparedMessage(message, messageType);
        std::cout<< "*" << messageToSend <<"*";
        _send(messageToSend);
    } catch (const std::runtime_error& error) {
        std::cerr << error.what() << std::endl;
    }
}

std::pair<Message*, Message*> Client::receive() {
    Message* header = _receiveMessage(HEADERSIZE, Message::Type::HEADER);
    Message* body = nullptr;
    if(header->getBodySize() != 0){
        body = _receiveMessage(header->getBodySize(), Message::Type::BODY);
    }

    return std::make_pair(header, body);
}

