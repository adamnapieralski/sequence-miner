#include "SequenceData.h"

#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "Parser.h"
#include "utils.hpp"

namespace {

std::vector<Sequence> mapValues(SequenceMap& map) {
  std::vector<Sequence> v;
  for (auto& it : map) {
    v.emplace_back(std::move(it.second));
  }
  return v;
}

std::unordered_map<int, int> itemsCounter(const std::vector<Sequence>& data) {
  std::unordered_map<int, int> counter;

  for (const auto& seq : data) {
    std::set<int> items;
    for (auto item : seq) {
      if (item != SEP) {
        items.insert(item);
      }
    }
    for (auto item : items) {
      ++counter[item];
    }
  }
  return counter;
}

std::set<int> infrequentItems(const std::vector<Sequence>& data,
                              int minSupport) {
  std::set<int> infrequentItems;
  for (auto& it : itemsCounter(data)) {
    if (it.second <= minSupport) {
      infrequentItems.insert(it.first);
    }
  }

  return infrequentItems;
}

std::set<int> frequentItems(const std::vector<Sequence>& data, int minSupport) {
  std::set<int> frequentItems;
  for (auto& it : itemsCounter(data)) {
    if (it.second > minSupport) {
      frequentItems.insert(it.first);
    }
  }
  return frequentItems;
}

}  // namespace

void SequenceData::load(const std::string& input, std::string separator,
                        std::string seq_separator, int limit) {
  std::cout << "Loading file " << input << std::endl;

  std::ifstream input_file{input};

  if (!input_file) {
    throw std::runtime_error("Input file not found");
  }

  SequenceMap seqs;

  if (input.substr(input.find_last_of('.') + 1) == "spmf") {
    inputDataType_ = DataType::t_int;
    seqs = parser::readSpfm(input_file, limit);
  } else {
    inputDataType_ = DataType::t_string;
    seqs = parser::readStringData(input_file, separator, seq_separator,
                                  stringToInt_, intToString_);
  }

  SequenceData d;
  data_ = mapValues(seqs);

  std::cout << "Loaded " << data_.size() << " sequences" << std::endl;
}

std::string SequenceData::getOriginalStringForId(int id) const {
  return intToString_.at(id);
}

int SequenceData::size() const { return static_cast<int>(data_.size()); }

const std::vector<int>& SequenceData::sequence(int id) const {
  return data_[id];
};

iterator SequenceData::begin() { return data_.begin(); }
iterator SequenceData::end() { return data_.end(); }

const_iterator SequenceData::begin() const { return data_.begin(); }
const_iterator SequenceData::end() const { return data_.end(); }

void SequenceData::clear() { data_.clear(); }

/**
 * Removes infrequent items from this data.
 */
int SequenceData::removeInfrequentItems(int min_support) {
  const auto items_to_delete = infrequentItems(data_, min_support);

  std::cout << "Removing " << items_to_delete.size() << " infrequent items"
            << std::endl;
  // for (auto item : items_to_delete) {
  //   std::cout << item << " ";
  // }
  // std::cout << std::endl;

  auto transform_function = [&items_to_delete](Sequence seq) {
    seq.erase(std::remove_if(seq.begin(), seq.end(),
                             [&items_to_delete](int x) {
                               return x != SEP && items_to_delete.find(x) !=
                                                      items_to_delete.end();
                             }),
              seq.end());

    if (seq.empty()) {
      return seq;
    }

    for (auto it = seq.begin() + 1; it != seq.end();) {
      if (*(it - 1) == SEP && *it == SEP) {
        it = seq.erase(it);
      } else {
        ++it;
      }
    }

    if (!seq.empty() && seq.front() == SEP) {
      seq.erase(seq.begin());
    }

    if (!seq.empty() && seq.back() == SEP) {
      seq.pop_back();
    }

    return seq;
  };

  std::transform(std::execution::par, data_.begin(), data_.end(), data_.begin(),
                 transform_function);

  return items_to_delete.size();
}

void SequenceData::printData() const {
  for (const auto& v : data_) {
    std::cout << "seq: " << utils::print(v) << std::endl;
  }
}

void SequenceData::setData(const std::vector<Sequence>& data) { data_ = data; }

void SequenceData::push_back(const Sequence& seq) { data_.push_back(seq); }

std::set<int> SequenceData::uniqueSingleItems() const {
  std::set<int> unique_items;

  for (const auto& v : data_) {
    for (auto s : v) {
      if (s != SEP) {
        unique_items.insert(s);
      }
    }
  }

  return unique_items;
}

/**
 * Creates and returns idList of single item (sequence of length 1)
 */
IdList_ SequenceData::getSingleItemIdList(int item) const {
  std::map<int, EidSequence> idList;
  int sid = 0;
  for (const auto& v : data_) {
    EidSequence eidSeq{};
    int eid = 0;
    for (auto s : v) {
      if (s == SEP) {
        ++eid;
      } else if (s == item) {
        eidSeq.insert(eid);
      }
    }
    if (!eidSeq.empty()) {
      idList.insert(std::pair<int, EidSequence>(sid, eidSeq));
    }
    ++sid;
  }
  return std::make_shared<IdList>(idList);
}

/**
 * Returns all frequent ECes with sequences of length 1.
 */
std::vector<EquivalenceClass_> SequenceData::getSingleFrequentItemClasses(
    int minSupport, bool withIdLists) const {
  const auto frequentItemsSet = frequentItems(data_, minSupport);

  std::vector<EquivalenceClass_> singleItemClasses;

  singleItemClasses.reserve(frequentItemsSet.size());
  for (const auto& item : frequentItemsSet) {
    singleItemClasses.push_back(
        std::make_shared<EquivalenceClass>(Sequence({item})));
  }

  if (withIdLists) {
    std::for_each(std::execution::par, singleItemClasses.begin(),
                  singleItemClasses.end(), [&](auto& singleClass) {
                    singleClass->setIdList(
                        getSingleItemIdList(singleClass->getSequence().at(0)));
                  });
  }

  return singleItemClasses;
}

/**
 * Updates seqClassMap with new sequence found at sid, eid position
 *
 * If class with such seq already exists in map, its idList is updated with sid,
 * eid. Otherwise new class in map is created with proper idList.
 */
void SequenceData::updateSeqClassMap(
    std::map<Sequence, EquivalenceClass_>& seqClassMap, const Sequence& seq,
    int sid, int eid) const {
  // if sequence already exists
  if (seqClassMap.find(seq) != seqClassMap.end()) {
    seqClassMap[seq]->addEidToSeqIdList(sid, eid);
  } else {
    seqClassMap.insert(std::pair<Sequence, EquivalenceClass_>(
        seq, std::make_shared<EquivalenceClass>(
                 seq, std::make_shared<IdList>(sid, eid))));
  }
}

/**
 * Returns all frequent ECes with sequences of length 2
 */
std::vector<EquivalenceClass_> SequenceData::getDoubleFrequentItemClasses(
    int minSupport) const {
  std::map<Sequence, EquivalenceClass_> seqClassMap;
  std::vector<EquivalenceClass_> doubleItemClasses;

  for (int sid = 0; sid < data_.size(); ++sid) {
    int eid = 0;
    for (int itId = 0; itId < data_[sid].size(); ++itId) {
      if (data_[sid][itId] == SEP) {
        ++eid;
        continue;
      }
      int eid2 = eid;
      for (int itId2 = itId + 1; itId2 < data_[sid].size(); ++itId2) {
        if (data_[sid][itId2] == SEP) {
          ++eid2;
          continue;
        }
        Sequence seq{data_[sid][itId]};
        if (eid2 > eid) {
          seq.push_back(SEP);
        }
        seq.push_back(data_[sid][itId2]);
        updateSeqClassMap(seqClassMap, seq, sid, eid2);
      }
    }
  }

  for (const auto& p : seqClassMap) {
    if (p.second->support() > minSupport) {
      doubleItemClasses.push_back(p.second);
    }
  }

  return doubleItemClasses;
}

DataType SequenceData::getInputDataType() const { return inputDataType_; }
