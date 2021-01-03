#pragma once

#include <iostream>

#include "Algorithm.h"

class SpadeAlgorithm : public Algorithm {
 public:
  SpadeAlgorithm() = default;
  bool loadData(SequenceData input) override;
  bool run(int min_support) override;

  void pushToFinalSequences(const std::vector<EquivalenceClass_>& eqClasses);
  void enumerateFrequentSequences(EquivalenceClass_& eq);

  void insertClassByPrefix(const EquivalenceClass_& eq, std::vector<EquivalenceClass_>& parents) const;
  void insertClassByPrefix(const EquivalenceClass_& eq, EquivalenceClass_& parent1, EquivalenceClass_& parent2) const;

  std::vector<EquivalenceClass_> generateCandidates(const EquivalenceClass_& eq1, const EquivalenceClass_& eq2) const;
  std::vector<EquivalenceClass_> generateJoinedCandidates(const EquivalenceClass_& eq1, const EquivalenceClass_& eq2) const;

 private:
  SequenceData input_;
};