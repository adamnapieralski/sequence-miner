#include "Algorithm.h"

#include <iostream>

#include "utils.hpp"

void Algorithm::printFinalSequences() const {
  std::cout << "Final sequences: ";
  std::lock_guard<std::mutex> lock(final_sequences_mutex_);
  for (const auto &seq : final_sequences_) {
    std::cout << utils::print(seq) << " ";
  }
  std::cout << std::endl;
  std::cout << "Size: " << final_sequences_.size() << std::endl;

}