#include <iostream>
#include <map>

#include "Algorithm.h"

struct Prefix : public std::vector<int> {
  Prefix(std::initializer_list<int> l) : std::vector<int>(l){};
};

class PrefixSpanAlgorithm : public Algorithm {
 public:
  PrefixSpanAlgorithm() = default;
  bool loadData(InputData input) override;
  bool run() override;

 private:
  void partitionAllSequences(const Prefix &prefix);
  void printSequences() const;

  std::map<Prefix, InputData> sequences_;
  std::vector<Prefix> finalSequences_;
};