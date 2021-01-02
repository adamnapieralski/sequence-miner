#pragma once

#include <iostream>

#include "Algorithm.h"

class SpadeAlgorithm : public Algorithm {
 public:
  SpadeAlgorithm() = default;
  bool loadData(SequenceData input) override;
  bool run(int min_support) override;

  void pushToFinalSequences(const std::vector<EquivalenceClass>& eqClasses);
  void enumerateFrequentSequences(EquivalenceClass& eq);

  void insertClassByPrefix(const EquivalenceClass& eq, std::vector<EquivalenceClass>& parents) const;
  void insertClassByPrefix(const EquivalenceClass& eq, EquivalenceClass& parent1, EquivalenceClass& parent2) const;

  std::vector<EquivalenceClass> generateCandidates(const EquivalenceClass& eq1, const EquivalenceClass& eq2) const;
  std::vector<EquivalenceClass> generateJoinedCandidates(const EquivalenceClass& eq1, const EquivalenceClass& eq2) const;



 private:
  SequenceData input_;
};