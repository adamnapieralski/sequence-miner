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

  EquivalenceClass(Sequence  seq);
  EquivalenceClass(Sequence  seq, IdList_  idList);

  void setIdList(const IdList_& idList);
  void insertToIdList(const std::pair<int, EidSequence>& seqIds);
  void addEidToSeqIdList(int sid, int eid);

  Sequence getSequence() const;
  IdList_ getIdList() const;

  void addMember(const EquivalenceClass_& member);
  void setMembers(const std::vector<EquivalenceClass_>& members);

  void removeMember(int id);

  std::vector<EquivalenceClass_> getMembers() const;
  int getMembersNum() const;

  int support() const;

  bool isParentOf(const EquivalenceClass_& eq) const;

  std::pair<Sequence, Sequence> getPrefixSuffixSeqParts() const;

  Sequence getLastSeqPair() const;

  void joinIdList(const EquivalenceClass_& eq1, const EquivalenceClass_& eq2);
  
  void print() const;

 private:
  Sequence seq_;
  IdList_ idList_;
  std::vector<EquivalenceClass_> members_;
};
