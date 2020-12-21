#pragma once

#include <string>

class InputData {
 public:
  InputData() = default;
  static InputData load(const std::string &input, char separator);
};
