#pragma once

#include <set>
#include <string>
#include <vector>
#include <map>

#include "EquivalenceClass.h"

enum class DataType { t_int, t_string };

using iterator = std::vector<std::vector<int>>::iterator;
using const_iterator = std::vector<std::vector<int>>::const_iterator;

class SequenceData {
 public:
  SequenceData() = default;

  SequenceData(const SequenceData &) = delete;
  SequenceData &operator=(const SequenceData &) = delete;

  SequenceData(SequenceData &&) = default;
  SequenceData &operator=(SequenceData &&) = default;

  void load(const std::string &input_path, std::string separator,
            std::string seq_separator, DataType type, int limit);

  std::string getOriginalStringForId(int id) const;

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

  template <class... Args>
  void emplace_back(Args &&... args) {
    data_.emplace_back(std::forward<Args>(args)...);
  }

  void push_back(const Sequence &seq);

  std::set<int> uniqueSingleItems() const;
  IdList_ getSingleItemIdList(int item) const;

  std::vector<EquivalenceClass_> getSingleFrequentItemClasses(
    int minSupport, bool withIdLists=false) const;

  void updateSeqClassMap(std::map<Sequence, EquivalenceClass_> &seqClassMap,
                         const Sequence &seq, int sid, int eid) const;
  std::vector<EquivalenceClass_> getDoubleFrequentItemClasses(
      int minSupport) const;

  DataType getInputDataType() const;

 private:
  std::vector<Sequence> data_;
  std::map<std::string, int> stringToInt_;
  std::map<int, std::string> intToString_;
  DataType inputDataType_;
};
