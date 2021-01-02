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
  IdList getIdList() const;

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
  IdList idList_;
  std::vector<EquivalenceClass> members_;
};