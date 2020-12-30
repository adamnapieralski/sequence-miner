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

Sequence EquivalenceClass::getSequence() const {
  return seq_;
}


void EquivalenceClass::print() const {

  std::cout << "Equivalence class for seq:\t";
  for (const auto& s : seq_) {
    std::cout << s << " ";
  }
  std::cout << std::endl;
  idList_.print();
}