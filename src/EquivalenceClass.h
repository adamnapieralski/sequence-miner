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

  void addMember(const EquivalenceClass &);

  void print() const;

 private:
  Sequence seq_;
  IdList idList_;
  std::vector<EquivalenceClass> members_;
};