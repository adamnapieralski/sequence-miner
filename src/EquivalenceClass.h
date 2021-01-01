#pragma once

#include <map>
#include <string>
#include <vector>

#include "IdList.h"

using Sequence = std::vector<int>;

class EquivalenceClass {
 public:
  EquivalenceClass() = default;
  EquivalenceClass(const Sequence &);
  EquivalenceClass(const Sequence &, const IdList &);

  void setIdList(const IdList &);
  void insertToIdList(const std::pair<int, EidSequence>);
  void addEidToSeqIdList(int sid, int eid);

  Sequence getSequence() const;

  void addMember(const EquivalenceClass &);

  int support() const;
  
  void print() const;

 private:
  Sequence seq_;
  IdList idList_;
  std::vector<EquivalenceClass> members_;
  // TODO add support
};