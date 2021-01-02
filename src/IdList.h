#pragma once

#include <map>
#include <string>
#include <vector>
#include <set>
#include <memory>

class IdList;

using EidSequence = std::set<int>;
using IdList_ = std::shared_ptr<IdList>;

class IdList {
 public:
  IdList() = default;
  IdList(const std::map<int, EidSequence> &);
  IdList(int sid, int eid);

  void insert(const std::pair<int, EidSequence>);
  void addEidToSeq(int sid, int eid);

  int size() const;

  std::map<int, EidSequence> getData() const;

  IdList_ joinEqual(IdList_ idList) const;

  IdList_ joinLatter(IdList_ idList) const;

  void clear();

  void print() const;

 private:
  std::map<int, EidSequence> data_;
};
