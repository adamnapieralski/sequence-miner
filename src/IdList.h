#pragma once

#include <map>
#include <string>
#include <vector>

using EidSequence = std::vector<int>;

class IdList {
 public:
  IdList() = default;
  IdList(const std::map<int, EidSequence> &);

  void insert(std::pair<int, EidSequence>);

  int size() const;

  void clear();

  void print() const;

 private:
  std::map<int, EidSequence> data_;
};
