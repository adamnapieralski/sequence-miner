#pragma once

#include <iostream>

#include "Algorithm.h"

class SpadeAlgorithm : public Algorithm {
 public:
  SpadeAlgorithm() = default;
  SpadeAlgorithm(bool dfs): dfs_{dfs} {};
  bool run(int minSupport) override;

  void setup(int minSupport, bool dfs);

  void pushToFinalSequences(const std::vector<EquivalenceClass_>& eqClasses);
  void enumerateFrequentSequences(const EquivalenceClass_& eq);

  static void insertClassByPrefix(const EquivalenceClass_& eq, std::vector<EquivalenceClass_>& parents) ;
  static void insertClassByPrefix(const EquivalenceClass_& eq, EquivalenceClass_& parent1, EquivalenceClass_& parent2) ;

  static std::vector<EquivalenceClass_> generateCandidates(const EquivalenceClass_& eq1, const EquivalenceClass_& eq2) ;
  static std::vector<EquivalenceClass_> generateJoinedCandidates(const EquivalenceClass_& eq1, const EquivalenceClass_& eq2) ;

 private:
  bool dfs_{false};
};