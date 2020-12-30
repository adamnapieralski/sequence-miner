#include "SpadeAlgorithm.h"

#include "SequenceData.h"

bool SpadeAlgorithm::loadData(SequenceData input) {
  std::cout << "Load data for SPADE algorithm" << std::endl;
  input_ =  std::move(input);
  return true;
}

bool SpadeAlgorithm::run(int min_support) {
  std::cout << "Run SPADE algorithm" << std::endl;
  min_support_ = min_support;
  input_.printData();
  input_.removeInfrequentItems(min_support_);
  auto frequentItems = input_.getSingleItemClasses();
  
  

  return true;
}