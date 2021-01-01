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
  // input_.removeInfrequentItems(min_support_);
  input_.printData();

  // auto root = new EquivalenceClass();
  auto frequentSingleItems = input_.getSingleFrequentItemClasses(min_support_);
  pushToFinalSequences(frequentSingleItems);

  auto frequentDoubleItems = input_.getDoubleFrequentItemClasses(min_support_);
  pushToFinalSequences(frequentDoubleItems);

  

  return true;
}

void enumerateFrequentSequences(const std::vector<EquivalenceClass>& doubleItems) {

}

void SpadeAlgorithm::pushToFinalSequences(const std::vector<EquivalenceClass>& eqClasses) {
  for (const auto& cl : eqClasses) {
    final_sequences_.push_back(cl.getSequence());
  }
}