#pragma once

#include <string>
#include <vector>

enum class DataType { t_int, t_char, t_string };

using iterator = std::vector<std::vector<int>>::iterator;
using const_iterator = std::vector<std::vector<int>>::const_iterator;

class InputData {
 public:
  InputData() = default;
  static InputData load(const std::string &input_path, char separator,
                        char seq_separator, DataType type);
  int size() const;
  const std::vector<int> &sequence(int id) const;

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

 private:
  std::vector<std::vector<int>> data_;
};
