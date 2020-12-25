#pragma once

#include <set>
#include <string>
#include <vector>

enum class DataType { t_int, t_char, t_string };
using Sequence = std::vector<int>;

using iterator = std::vector<std::vector<int>>::iterator;
using const_iterator = std::vector<std::vector<int>>::const_iterator;

class InputData {
 public:
  InputData() = default;

  InputData(const InputData &) = delete;
  InputData &operator=(const InputData &) = delete;

  InputData(InputData &&) = default;
  InputData &operator=(InputData &&) = default;

  static InputData load(const std::string &input_path, char separator,
                        char seq_separator, DataType type);
  int size() const;
  const std::vector<int> &sequence(int id) const;

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  int removeInfrequentItems(int min_support);

  void printData() const;

  void setData(const std::vector<Sequence> &data);
  void push_back(const Sequence &seq);

  std::set<int> uniqueSingleItems() const;

 private:
  std::vector<Sequence> data_;
};
