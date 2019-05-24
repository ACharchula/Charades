// Copyright 2019 Kamil Mankowski

#ifndef SRC_SERVER_COMMANDS_HELPERS_H_
#define SRC_SERVER_COMMANDS_HELPERS_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using buffer_t = std::vector<char>;
using buffer_ptr = std::shared_ptr<buffer_t>;

namespace helpers {
buffer_ptr to_buf(const std::string &data);
std::string to_str(buffer_ptr data);
buffer_ptr get_zero_width_size(int size, int width = 4);
void log(const std::string &msg, int user = -1);
}  // namespace helpers

#endif  // SRC_SERVER_COMMANDS_HELPERS_H_
