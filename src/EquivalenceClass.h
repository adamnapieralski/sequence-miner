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

  EquivalenceClass(const EquivalenceClass &) = delete;
  EquivalenceClass &operator=(const EquivalenceClass &) = delete;

  EquivalenceClass(EquivalenceClass &&) = default;
  EquivalenceClass &operator=(EquivalenceClass &&) = default;

  EquivalenceClass(const Sequence& seq);
  EquivalenceClass(const Sequence& seq, IdList_ idList);

  void setIdList(IdList_ idList);
  void insertToIdList(const std::pair<int, EidSequence>);
  void addEidToSeqIdList(int sid, int eid);

  Sequence getSequence() const;
  IdList_ getIdList() const;

  void addMember(EquivalenceClass_ member);
  void setMembers(std::vector<EquivalenceClass_> members);

  std::vector<EquivalenceClass_> getMembers();
  int getMembersNum() const;

  int support() const;

  bool isParentOf(EquivalenceClass_ eq) const;

  std::pair<Sequence, Sequence> getPrefixSuffixSeqParts() const;

  Sequence getLastSeqPair() const;

  void joinIdList(EquivalenceClass_ eq1, EquivalenceClass_ eq2);
  
  void print() const;

 private:
  Sequence seq_;
  IdList_ idList_;
  std::vector<EquivalenceClass_> members_;
};