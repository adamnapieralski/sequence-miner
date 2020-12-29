#pragma once

#include <iostream>

#include "Algorithm.h"

class SpadeAlgorithm : public Algorithm {
 public:
  SpadeAlgorithm() = default;
  bool loadData(SequenceData input) override;
  bool run(int min_support) override;

 private:
  SequenceData input_;
};