#include "EquivalenceClass.h"

#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "SequenceData.h"
#include "utils.hpp"

EquivalenceClass::EquivalenceClass(Sequence seq) : seq_{std::move(seq)} {}

EquivalenceClass::EquivalenceClass(Sequence seq, IdList_ idList)
    : seq_{std::move(seq)}, idList_{std::move(idList)} {}

void EquivalenceClass::setIdList(const IdList_& idList) { idList_ = idList; }

void EquivalenceClass::insertToIdList(
    const std::pair<int, EidSequence>& seqIds) {
  idList_->insert(seqIds);
}

void EquivalenceClass::addEidToSeqIdList(int sid, int eid) {
  idList_->addEidToSeq(sid, eid);
}

Sequence EquivalenceClass::getSequence() const { return seq_; }

IdList_ EquivalenceClass::getIdList() const { return idList_; }

void EquivalenceClass::addMember(const EquivalenceClass_& member) {
  members_.push_back(member);
}

void EquivalenceClass::setMembers(
    const std::vector<EquivalenceClass_>& members) {
  members_ = members;
}

std::vector<EquivalenceClass_> EquivalenceClass::getMembers() const {
  return members_;
}

void EquivalenceClass::removeMember(int id) {
  members_.erase(members_.begin() + id);
}

int EquivalenceClass::support() const { return idList_->size(); }

/**
 * Is "parent" (or predecessor) of other EC if this sequence is a subset of eq's
 * sequence from the beginning
 */
bool EquivalenceClass::isParentOf(const EquivalenceClass_& eq) const {
  return std::equal(seq_.begin(), seq_.end(), eq->getSequence().cbegin());
}

/**
 * Returns two last atoms with seperators (=SEP) between them and to the left,
 * if applicable One of cases: {[A, B], [SEP, A, B], [A, SEP, B], [SEP, A, SEP,
 * B]}
 */
Sequence EquivalenceClass::getLastSeqPair() const {
  int rStartId = 1;
  bool found = false;
  if (seq_.size() < 2) {
    throw std::out_of_range("Sequence has less than 2 elements.");
  }
  for (int i = seq_.size() - 2; i >= 0; --i) {
    if (seq_[i] == SEP) {
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

/**
 * Returns pair of <prefix, suffix>, where suffix would be last atom with left
 * separator (=SEP) if applicable suffix = {[SEP, S], [S]}, prefix = everything
 * before it
 */
std::pair<Sequence, Sequence> EquivalenceClass::getPrefixSuffixSeqParts()
    const {
  if (seq_.size() > 1) {
    int split = 1;
    // P -> S
    if (seq_[seq_.size() - 2] == SEP) {
      split = 2;
    }
    // else PS - split stays = 1
    return std::pair<Sequence, Sequence>(
        Sequence(seq_.begin(), seq_.end() - split),
        Sequence(seq_.end() - split, seq_.end()));
  }
  if (seq_.size() == 1) {
    return std::pair<Sequence, Sequence>(Sequence(), seq_);
  }
  throw std::out_of_range("Empty sequence");
}

/**
 * Sets this idList as a joined one out of eq1 and eq2 idLists
 *
 * Handles different suffixes configurations.
 */
void EquivalenceClass::joinIdList(const EquivalenceClass_& eq1,
                                  const EquivalenceClass_& eq2) {
  const auto lastPair = getLastSeqPair();
  const auto presuf1 = eq1->getPrefixSuffixSeqParts();
  const auto presuf2 = eq2->getPrefixSuffixSeqParts();

  // PSS
  if (lastPair.size() == 2) {
    idList_ = eq1->idList_->joinEqual(eq2->idList_);
  }
  // P -> SS
  else if (lastPair.size() == 3 && lastPair[0] == SEP) {
    idList_ = eq1->idList_->joinEqual(eq2->idList_);
  }
  // PS -> S
  else if (lastPair.size() == 3 && lastPair[1] == SEP) {
    // PS1 -> S2
    if (presuf1.second.size() == 1 && presuf2.second.size() == 2 &&
        presuf2.second[0] == SEP) {
      idList_ = eq1->idList_->joinLatter(eq2->idList_);
    }
    // PS2 -> S1
    else if (presuf2.second.size() == 1 && presuf1.second.size() == 2) {
      idList_ = eq2->idList_->joinLatter(eq1->idList_);
    }
  }
  // P -> S -> S
  else if (lastPair.size() == 4 && presuf1.second.size() == 2 &&
           presuf2.second.size() == 2) {
    // P -> S1 -> S2
    if (lastPair[1] == presuf1.second[1]) {
      idList_ = eq1->idList_->joinLatter(eq2->idList_);
    }
    // P -> S2 -> S1
    else if (lastPair[1] == presuf2.second[1]) {
      idList_ = eq2->idList_->joinLatter(eq1->idList_);
    }
  } else {
    throw std::invalid_argument("Invalid last pair of this class.");
  }
}

void EquivalenceClass::print() const {
  std::cout << "Equivalence class for seq:\t";
  for (const auto& s : seq_) {
    std::cout << s << " ";
  }
  std::cout << std::endl;
  idList_->print();

  std::cout << "Members:" << std::endl;
  for (const auto& m : members_) {
    for (const auto& s : m->getSequence()) {
      std::cout << s << " ";
    }
    std::cout << std::endl;
  }
}
