#include "Algorithm.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "utils.hpp"

void Algorithm::printFinalSequences() {
  std::cout << "Final sequences: ";
  std::lock_guard<std::mutex> lock(final_sequences_mutex_);
  std::sort(final_sequences_.begin(), final_sequences_.end());
  for (const auto &seq : final_sequences_) {
    std::cout << utils::print(seq) << " ";
  }
  std::cout << std::endl;
  std::cout << "Size: " << final_sequences_.size() << std::endl;
}

void Algorithm::exportFinalSequences(std::ofstream &f) {
  std::lock_guard<std::mutex> lock(final_sequences_mutex_);
  std::sort(final_sequences_.begin(), final_sequences_.end());
  for (const auto &seq : final_sequences_) {
    for (auto elem : seq) {
      f << elem << " ";
    }
    f << "\n";
  }
}