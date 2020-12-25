#include "PrefixSpanAlgorithm.h"

#include "InputData.h"

namespace {
const Prefix null_prefix{0};
}

bool PrefixSpanAlgorithm::loadData(InputData input) {
  std::cout << "Load data for PrefixSpan algorithm" << std::endl;
  input.printData();

  sequences_[null_prefix] = std::move(input);
  return true;
}

bool PrefixSpanAlgorithm::run() {
  std::cout << "Run PrefixSpan algorithm" << std::endl;

  auto &input = sequences_[null_prefix];

  input.removeInfrequentItems(1);
  input.printData();

  partitionAllSequences(null_prefix);

  printSequences();

  return true;
}

void PrefixSpanAlgorithm::partitionAllSequences(const Prefix &prefix) {
  auto &data = sequences_[prefix];

  auto single_items = data.uniqueSingleItems();

  std::cout << "unique items: ";
  for (auto item : single_items) {
    finalSequences_.push_back(Prefix{item});
    std::cout << item << " ";
  }
  std::cout << std::endl;

  for (Sequence &seq : data) {
    if (!seq.empty()) {
      auto prefix = Prefix{seq.front()};
      seq.erase(seq.begin());
      if (!seq.empty()) {
        sequences_[prefix].push_back(seq);
      }
    }
  }
}

void PrefixSpanAlgorithm::printSequences() const {
  for (auto it = sequences_.begin(); it != sequences_.end(); ++it) {
    std::cout << "********************************************" << std::endl;
    std::cout << "sequence " << it->first[0] << std::endl;
    it->second.printData();
  }
  std::cout << "********************************************" << std::endl;
}
