//
// Created by adam on 13.04.19.
//

#include <stdexcept>
#include <iostream>
#include "Message.h"

Message::Message(size_t size) : size(size) {}

void Message::append(std::pair<char*, ssize_t> nextData) {
    if (currentSize + nextData.second > size)
      throw std::runtime_error("Error in receiving data.");
    value.append(nextData.first, nextData.second);
    currentSize += nextData.second - 1;
}

const size_t Message::getSize() const {
    return static_cast<const size_t>(stoi(value.substr(0, value.size())));
}

void Message::print() const {
    std::cout << value << std::endl;
}

bool Message::equal (std::string other) const {
    return value == other;
}

const std::string& Message::getValue() const {
    return value;
}

const std::string Message::getWinnerTextMessage() const {
    std::string msg = "Correct answer-> ";
    msg.append(getTextMessage());
    return msg;
}

const std::string Message::getTextMessage() const {
    size_t i;
    for(i = 0; value[i] != '\n'; ++i){
        ;
    }

    std::string textMessage = value.substr(0, i);
    textMessage.append(": ");
    textMessage.append(value.substr(i+1, value.size()));

    return textMessage;
}
