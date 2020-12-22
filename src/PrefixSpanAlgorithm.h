#include <iostream>

#include "Algorithm.h"

class PrefixSpanAlgorithm : public Algorithm {
 public:
  PrefixSpanAlgorithm() = default;
  bool loadData(InputData input) override;
  bool run() override;
};