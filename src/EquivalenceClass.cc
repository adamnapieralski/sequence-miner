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