//
// Created by adam on 13.04.19.
//

#ifndef CPP_CLIENT_MESSAGE_H
#define CPP_CLIENT_MESSAGE_H


#include <string>

class Message {

private:

    std::string value;
    size_t size;
    size_t currentSize = 0;

public:
    Message(size_t size);

    void append(std::pair<char*, ssize_t> nextData);

    const std::string& getValue() const;

    const std::string getTextMessage() const;

    const std::string getClue() const;

    const std::string getAborted() const;

    const size_t getSize() const;

    void print() const;

    bool equal (std::string other) const;
};


#endif //CPP_CLIENT_MESSAGE_H
