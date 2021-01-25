#pragma once

#include <mutex>

#include "SequenceData.h"

class Algorithm {
 public:
  virtual bool run(int minSupport) = 0;

  bool loadData(const std::string& input_path, std::string separator, std::string seq_separator, int limit);

  std::vector<std::vector<std::string>> getFinalSequencesAsOriginalStrings(std::string eventsSep);

  void printFinalSequences();
  void exportFinalSequences(std::ofstream& f, std::string itemsSep, std::string eventsSep);

 protected:
  int min_support_{1};
  SequenceData input_;
  std::vector<Sequence> final_sequences_;
  mutable std::mutex final_sequences_mutex_;
};