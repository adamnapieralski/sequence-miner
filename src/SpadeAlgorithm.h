#pragma once

#include <iostream>

#include "Algorithm.h"

class SpadeAlgorithm : public Algorithm {
 public:
  SpadeAlgorithm() = default;
  bool loadData(SequenceData input) override;
  bool run(int min_support) override;

  void pushToFinalSequences(const std::vector<EquivalenceClass>& eqClasses);


 private:
  SequenceData input_;
};