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

  IdList(const IdList &) = delete;
  IdList &operator=(const IdList &) = delete;

  IdList(IdList &&) = default;
  IdList &operator=(IdList &&) = default;

  IdList(const std::map<int, EidSequence>& data);
  IdList(int sid, int eid);

  void insert(const std::pair<int, EidSequence>& seqEids);
  void addEidToSeq(int sid, int eid);

  int size() const;

  std::map<int, EidSequence> getData() const;

  IdList_ joinEqual(const IdList_& idList) const;

  IdList_ joinLatter(const IdList_& idList) const;

  void clear();

  void print() const;

 private:
  std::map<int, EidSequence> data_;
};
