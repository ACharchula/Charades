//
// Created by adam on 13.04.19.
//

#include <stdexcept>
#include <iostream>
#include "Message.h"

Message::Message(size_t size, Message::Type type) : size(size), type(type) {}

void Message::append(std::pair<char*, ssize_t> nextData) {
    if (currentSize + nextData.second > size)
        throw std::runtime_error("Error in receiving data.");
    value.append(nextData.first, nextData.second);
    currentSize += nextData.second - 1;
}

const size_t Message::getBodySize() const {
    if (type != HEADER)
        return 0;
    else
        return stoi(value.substr(12, 15));
}

void Message::print() const {
    std::cout << value << std::endl;
}

void Message::endMessage() {
    value += '\0';
}
