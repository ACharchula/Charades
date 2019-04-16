// Copyright 2019 Kamil Mankowski

#include "Helpers.h"

namespace helpers {
std::string get_zero_width_size(int size) {
  std::string out = std::to_string(size);
  while (out.size() < 4) {
    out = "0" + out;
  }
  return out;
}
}  // namespace helpers