//
// Created by adam on 13.04.19.
//

#ifndef CPP_CLIENT_MESSAGE_H
#define CPP_CLIENT_MESSAGE_H


#include <string>

class Message {
public:
    enum Type {
        HEADER,
        BODY
    };

private:

    std::string value;
    Type type;
    size_t size;
    size_t currentSize = 0;

public:
    Message(size_t size, Message::Type type);

    void append(std::pair<char*, ssize_t> nextData);

    void endMessage();

    const size_t getBodySize() const;

    void print() const;
};


#endif //CPP_CLIENT_MESSAGE_H
