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

//client
const std::string TEXT = "SEND_MESSAGE";
const std::string HELLO = "HELLO_SERVER";
const std::string ENTER = "ENTER__TABLE";

//server
const std::string WELCOME = "WELCOME_USER"; //4
const std::string CHAT = "CHAT_MESSAGE"; // 4
const std::string WAIT = "GAME_WAITING"; // 4
const std::string END = "GAME___ENDED"; // 4
const std::string READY = "GAME___READY"; // 4
const std::string DRAW = "YOUAREDRAWER"; // 4
const std::string CORRECT = "CLUE_CORRECT"; // 4
const std::string INCORRECT = "CLUEINCORECT"; // 4
const std::string SET = "SET___CANVAS"; // 8
const std::string UPDATE = "UPDATECANVAS"; // 8

class Client {
    int sock;
    struct sockaddr_in server;
    struct hostent* hp;
    const std::string userName;

    void _createSocket();

    void _connect(const char* serverName, unsigned port);

    void _send(const char* message, size_t messageSize);

    std::string _getMessageSize(size_t size, const std::string messageType);

    std::pair<const char*, size_t> _preparedMessage(const std::string message, const std::string messageType);

    std::pair<char*, ssize_t> _receive(size_t expectedDataSize); // first- header, second- body

    Message* _receiveMessage(size_t expectedDataSize);

public:
    Client(const std::string userName);

    ~Client();

    void run(const char* serverName, unsigned port);

    void send(const std::string message, const std::string messageType);

    std::pair<Message*, Message*> receive();
};


#endif //CPP_CLIENT_CLIENT_H
