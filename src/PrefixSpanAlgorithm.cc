#include "PrefixSpanAlgorithm.h"

#include "InputData.h"

bool PrefixSpanAlgorithm::loadData(InputData input) {
  std::cout << "Load data for PrefixSpan algorithm" << std::endl;
  input.printData();
  input.removeInfrequentItems(1);
  input.printData();

  return true;
}

bool PrefixSpanAlgorithm::run() {
  std::cout << "Run PrefixSpan algorithm" << std::endl;
  return true;
}