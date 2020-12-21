#include "InputData.h"

#include <stdexcept>

InputData InputData::load(const std::string &input, char separator) {
  FILE *f = fopen(input.c_str(), "r");
  if (f == nullptr) {
    throw std::runtime_error("Input file not found");
  }

  fclose(f);

  InputData d;
  return d;
}