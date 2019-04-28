// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_HELPERS_H_
#define SRC_SERVER_COMMANDS_HELPERS_H_

#include <iostream>
#include <string>

namespace helpers {
std::string get_zero_width_size(int size);
void log(const std::string &msg, int user = -1);
}  // namespace helpers

#endif  // SRC_SERVER_COMMANDS_HELPERS_H_
