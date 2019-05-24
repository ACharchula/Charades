// Copyright 2019 Kamil Mankowski

#include "Helpers.h"

namespace helpers {
buffer_ptr get_zero_width_size(int size, int width) {
  std::string out = std::to_string(size);
  while (out.size() < width) {
    out = "0" + out;
  }
  return to_buf(out);
}

void log(const std::string &msg, int userid) {
  std::cout << "LOG";
  if (userid >= 0) std::cout << " [" << userid << "]";
  std::cout << ": " << msg << std::endl;
}

buffer_ptr to_buf(const std::string &data) {
  auto buff = std::make_shared<std::vector<char>>();
  buff->reserve(data.size());
  buff->assign(data.begin(), data.end());
  return buff;
}

std::string to_str(buffer_ptr data) {
  std::string out(data->data(), data->size());
  return out;
}
}  // namespace helpers
