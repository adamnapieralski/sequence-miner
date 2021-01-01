#include <iostream>
#include <map>
#include <mutex>

#include "Algorithm.h"

struct Prefix : public std::vector<int> {
  Prefix() = default;
  Prefix(std::initializer_list<int> l) : std::vector<int>(l){};
};

class PrefixSpanAlgorithm : public Algorithm {
 public:
  PrefixSpanAlgorithm() = default;
  bool loadData(SequenceData input) override;
  bool run(int min_support) override;

 private:
  /// Split input data into projected datasets for each prefix.
  std::map<Prefix, SequenceData> partitionAllSequences();
  void printSequences(const std::map<Prefix, SequenceData> &sequences) const;
  void printFinalSequences() const;

  /// Get fequent items in the given dataset. Prefix is needed to properly
  /// identify sufixes.
  std::vector<int> frequentItems(const Prefix &prefix,
                                 const SequenceData &data) const;

  /// Main algorithm method. It recursively divides datasets into subsets, where
  /// each subset has the given prefix.
  void recursiveSolve(const Prefix &prefix, const SequenceData &data);

  void appendFinalSequences(const Prefix &prefix,
                            const std::vector<int> &items);

  mutable std::mutex final_sequences_mutex_;
};