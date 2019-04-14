//
// Created by adam on 13.04.19.
//

#ifndef CPP_CLIENT_MESSAGE_H
#define CPP_CLIENT_MESSAGE_H


#include <string>

class Message {
public:
    enum Type{
        HEADER,
        BODY
    };

private:

    std::string value;
    Type type;
    size_t size;
    size_t currentSize = 0;

public:
    Message(size_t size);

    Message(std::string& value, Type type, size_t size);

    size_t getSize() const;

    size_t getCurrentSize() const;

    void append(std::pair<char*, ssize_t> nextData);

    const std::string& getValue() const;
};


#endif //CPP_CLIENT_MESSAGE_H
