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
    return stoi(value.substr(0, value.size()));
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
