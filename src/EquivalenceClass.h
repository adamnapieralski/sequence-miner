#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "IdList.h"

class EquivalenceClass;

using Sequence = std::vector<int>;
using EquivalenceClass_ = std::shared_ptr<EquivalenceClass>;

class EquivalenceClass {
 public:
  EquivalenceClass() = default;
  EquivalenceClass(const Sequence& seq);
  EquivalenceClass(const Sequence& seq, IdList_ idList);

  void setIdList(IdList_ idList);
  void insertToIdList(const std::pair<int, EidSequence>);
  void addEidToSeqIdList(int sid, int eid);

  Sequence getSequence() const;
  IdList_ getIdList() const;

  void addMember(const EquivalenceClass& member);
  void setMembers(const std::vector<EquivalenceClass>& members);

  std::vector<EquivalenceClass>& getMembers();
  int getMembersNum() const;

  int support() const;

  bool isParentOf(const EquivalenceClass& eq) const;

  std::pair<Sequence, Sequence> getPrefixSuffixSeqParts() const;

  Sequence getLastSeqPair() const;

  void joinIdList(const EquivalenceClass& eq1, const EquivalenceClass& eq2);
  
  void print() const;

 private:
  Sequence seq_;
  IdList_ idList_;
  std::vector<EquivalenceClass> members_;
  EquivalenceClass_ child;
};