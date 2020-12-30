#pragma once

#include <set>
#include <string>
#include <vector>

#include "EquivalenceClass.h"

enum class DataType { t_int, t_char };

using iterator = std::vector<std::vector<int>>::iterator;
using const_iterator = std::vector<std::vector<int>>::const_iterator;

class SequenceData {
 public:
  SequenceData() = default;

  SequenceData(const SequenceData &) = delete;
  SequenceData &operator=(const SequenceData &) = delete;

  SequenceData(SequenceData &&) = default;
  SequenceData &operator=(SequenceData &&) = default;

  static SequenceData load(const std::string &input_path, char separator,
                           char seq_separator, DataType type);
  int size() const;
  const std::vector<int> &sequence(int id) const;

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  void clear();

  int removeInfrequentItems(int min_support);

  void printData() const;

  void setData(const std::vector<Sequence> &data);
  void push_back(const Sequence &seq);

  std::set<int> uniqueSingleItems() const;
  IdList getSingleItemIdList(int) const;
  std::vector<EquivalenceClass> getSingleItemClasses() const;

  void updateSeqClassMap(std::map<Sequence, EquivalenceClass>& seqClassMap, Sequence& seq, int sid, int eid) const;
  std::vector<EquivalenceClass> getDoubleFrequentItemClasses(int minSupport) const;

 private:
  std::vector<Sequence> data_;
};
