#pragma once

#include <mutex>

#include "SequenceData.h"

class Algorithm {
 public:
  virtual bool loadData(SequenceData input) = 0;
  virtual bool run(int min_support) = 0;

  void printFinalSequences();
  void exportFinalSequences(std::ofstream& f);

 protected:
  int min_support_{1};
  SequenceData input_;
  std::vector<Sequence> final_sequences_;
  mutable std::mutex final_sequences_mutex_;
};