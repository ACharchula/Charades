//
// Created by adam on 13.04.19.
//

#include <stdexcept>
#include <iostream>
#include "Message.h"

Message::Message(std::string& value, Message::Type type, size_t size) : value(value), type(type), size(size) {}

Message::Message(size_t size, Message::Type type) : size(size), type(type){}

size_t Message::getSize() const {
    return size;
}

void Message::append(std::pair<char*, ssize_t> nextData) {
    if(currentSize + nextData.second > size)
        throw std::runtime_error("Error in receiving data.");
    value.append(nextData.first, nextData.second);
    currentSize += nextData.second -1;
}

size_t Message::getCurrentSize() const {
    return currentSize;
}

const std::string& Message::getValue() const {
    return value;
}

const size_t Message::getBodySize() const {
    if (type != HEADER)
        return 0;
    else
        return stoi(value.substr(12,15));
}

void Message::print() const {
    std::cout <<value <<std::endl;
}
