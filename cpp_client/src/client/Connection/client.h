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

const size_t HEADERSIZE = 12;
const size_t SHORT = 4;
const size_t LONG = 8;

class Client {
    int sock;
    struct sockaddr_in server;
    struct hostent* hp;
    const std::string userName;

    void _createSocket();

    void _connect(const char* serverName, uint16_t port);

    void _send(const char* message, size_t messageSize);

    std::string _getMessageSize(size_t size, std::string messageType);

    std::pair<const char*, size_t> _preparedMessage(std::string message, std::string messageType);

    std::pair<char*, ssize_t> _receive(size_t expectedDataSize); // first- header, second- body

    Message* _receiveMessage(size_t expectedDataSize);

public:
    explicit Client(std::string userName);

    ~Client();

    void run(const char* serverName, uint16_t port);

    void send(std::string message, std::string messageType);

    std::pair<Message*, Message*> receive();
};


#endif //CPP_CLIENT_CLIENT_H
