#include "EquivalenceClass.h"

#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "utils.hpp"

EquivalenceClass::EquivalenceClass(const Sequence& seq) {
  seq_ = seq;
}

EquivalenceClass::EquivalenceClass(const Sequence& seq, const IdList& idList) {
  seq_ = seq;
  idList_ = idList;
}

void EquivalenceClass::setIdList(const IdList & idList) {
  idList_ = idList;
}

void EquivalenceClass::insertToIdList(const std::pair<int, EidSequence> seqIds) {
  idList_.insert(seqIds);
}

void EquivalenceClass::addEidToSeqIdList(int sid, int eid) {
  idList_.addEidToSeq(sid, eid);
}

Sequence EquivalenceClass::getSequence() const {
  return seq_;
}

IdList EquivalenceClass::getIdList() const {
  return idList_;
}


void EquivalenceClass::addMember(const EquivalenceClass& member) {
  members_.push_back(member);
}

void EquivalenceClass::setMembers(const std::vector<EquivalenceClass>& members) {
  members_ = members;
}

std::vector<EquivalenceClass>& EquivalenceClass::getMembers() {
  return members_;
}

int EquivalenceClass::getMembersNum() const {
    return members_.size();
  }

int EquivalenceClass::support() const {
  return idList_.size();
}

bool EquivalenceClass::isParentOf(const EquivalenceClass& eq) const {
  return std::equal(seq_.begin(), seq_.end(), eq.getSequence().begin());
}

Sequence EquivalenceClass::getLastSeqPair() const {
  int rStartId = 1;
  bool found = false;
  if (seq_.size() < 2) {
    throw std::out_of_range("Sequence has less than 2 elements.");
  }
  for (int i = seq_.size() - 2; i >= 0; --i) {
    if (seq_[i] == -1) {
      ++rStartId;
      if (found) {
        break;
      }
    } else {
      if (found) {
        break;
      }
      found = true;
      ++rStartId;
    }
  }
  return Sequence(seq_.end() - rStartId, seq_.end());
}


std::pair<Sequence, Sequence> EquivalenceClass::getPrefixSuffixSeqParts() const {
  if (seq_.size() > 1) {
    int split = 1;
    // P -> S
    if (seq_[seq_.size() - 2] == -1) {
      split = 2;
    }
    // else PS - split stays = 1
    return std::pair<Sequence, Sequence>(Sequence(seq_.begin(), seq_.end() - split), Sequence(seq_.end() - split, seq_.end()));
  } else if (seq_.size() == 1) {
    return std::pair<Sequence, Sequence>(Sequence(), seq_);
  }
  throw std::out_of_range("Empty sequence");
}


void EquivalenceClass::joinIdList(const EquivalenceClass& eq1, const EquivalenceClass& eq2) {
  auto lastPair = getLastSeqPair();
  auto presuf1 = eq1.getPrefixSuffixSeqParts();
  auto presuf2 = eq2.getPrefixSuffixSeqParts();

  // PSS
  if (lastPair.size() == 2) {
    idList_ = eq1.idList_.joinEqual(eq2.idList_);
  }
  // P -> SS
  else if (lastPair.size() == 3 && lastPair[0] == -1) {
    idList_ = eq1.idList_.joinEqual(eq2.idList_);
  }
  // PS -> S
  else if (lastPair.size() == 3 && lastPair[1] == -1) {
    // PS1 -> S2
    if (presuf1.second.size() == 1 && presuf2.second.size() == 2 && presuf2.second[0] == -1) {
      idList_ = eq1.idList_.joinLatter(eq2.idList_);
    }
    // PS2 -> S1
    else if (presuf2.second.size() == 1 && presuf1.second.size() == 2) {
      idList_ = eq2.idList_.joinLatter(eq1.idList_);
    }
  }
  // P -> S -> S
  else if (lastPair.size() == 4 && presuf1.second.size() == 2 && presuf2.second.size() == 2) {
    // P -> S1 -> S2
    if (lastPair[1] == presuf1.second[1]) {
      idList_ = eq1.idList_.joinLatter(eq2.idList_);
    }
    // P -> S2 -> S1
    else if (lastPair[1] == presuf2.second[1]) {
      idList_ = eq2.idList_.joinLatter(eq1.idList_);
    }
  }
  else {
    throw std::invalid_argument("Invalid last pair of this class.");
  }
}



void EquivalenceClass::print() const {

  std::cout << "Equivalence class for seq:\t";
  for (const auto& s : seq_) {
    std::cout << s << " ";
  }
  std::cout << std::endl;
  idList_.print();

  std::cout << "Members:" << std::endl;
  for (const auto& m : members_) {
    for (const auto& s : m.getSequence()) {
      std::cout << s << " ";
    }
    std::cout << std::endl;
  }
}