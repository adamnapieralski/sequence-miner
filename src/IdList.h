#pragma once

#include <map>
#include <string>
#include <vector>

using EidSequence = std::vector<int>;

class IdList {
 public:
  IdList() = default;
  IdList(const std::map<int, EidSequence> &);
  IdList(int sid, int eid);

  void insert(const std::pair<int, EidSequence>);
  void addEidToSeq(int sid, int eid);

  int size() const;

  void clear();

  void print() const;

 private:
  std::map<int, EidSequence> data_;
};
