#include "Algorithm.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "utils.hpp"


bool Algorithm::loadData(const std::string& input_path, std::string separator,
                         std::string seq_separator, DataType type, int limit) {
  input_.load(input_path, separator, seq_separator, type, limit);
  return true;
}

std::vector<std::vector<std::string>> Algorithm::getFinalSequencesAsOriginalStrings() {
  std::vector<std::vector<std::string>> originalStringsSequences;

  std::lock_guard<std::mutex> lock(final_sequences_mutex_);
  std::sort(final_sequences_.begin(), final_sequences_.end());

  for (const auto& seq : final_sequences_) {
    std::vector<std::string> strSeq;
    for (auto elem : seq) {
      if (elem != -1) {
        strSeq.push_back(input_.getOriginalStringForId(elem));
      }
    }
    originalStringsSequences.push_back(strSeq);
  }
  return originalStringsSequences;
}

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

void Algorithm::exportFinalSequences(std::ofstream &f, std::string sep) {
  if (input_.getInputDataType() == DataType::t_string) {
    for (const auto& seq : getFinalSequencesAsOriginalStrings()) {
      for (const auto& elem : seq) {
        f << elem << sep;
      }
      f << "\n";
    }
  } else if (input_.getInputDataType() == DataType::t_int) {
    std::lock_guard<std::mutex> lock(final_sequences_mutex_);
    std::sort(final_sequences_.begin(), final_sequences_.end());
    for (const auto& seq : final_sequences_) {
      for (const auto& elem : seq) {
        f << elem << sep;
      }
      f << "\n";
    }
  } else {
    std::cout << "Cannot export results. Invalid format." << std::endl;
  }
}