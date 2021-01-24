#include "PrefixSpanAlgorithm.h"

#include <algorithm>
#include <chrono>
#include <execution>
#include <sstream>
#include <unordered_map>

#include "Algorithm.h"
#include "SequenceData.h"
#include "utils.hpp"

using time_point = std::chrono::steady_clock::time_point;

namespace {
const Prefix null_prefix{0};

bool checkSeq(const Prefix &pref, const Sequence &seq, int pos) {
  --pos;
  for (auto it = pref.rbegin(); it != pref.rend(); ++it) {
    if (pos >= 0 && seq[pos] == SEP) {
      return false;
    }
    while (pos >= 0 && *it != seq[pos]) {
      if (seq[pos] == SEP) {
        return false;
      }
      --pos;
    }
    --pos;
  }
  return pos >= 0;
}
}  // namespace

bool PrefixSpanAlgorithm::run(int minSupport) {
  time_point begin = std::chrono::steady_clock::now();

  std::cout << "Run PrefixSpan algorithm" << std::endl;

  min_support_ = minSupport;

  input_.removeInfrequentItems(min_support_);
  // input_.printData();

  const auto sequences = partitionAllSequences();

  // no longer needed
  input_.clear();

  std::for_each(
      std::execution::par, sequences.cbegin(), sequences.cend(),
      [=](const auto &item) { recursiveSolve(item.first, item.second); });

  time_point end = std::chrono::steady_clock::now();
  std::cout << "Execution time = "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     begin)
                   .count()
            << " [ms]" << std::endl;

  // printFinalSequences();
  std::cout << "Found " << final_sequences_.size() << " frequent sequences"
            << std::endl;

  return true;
}

std::map<Prefix, SequenceData> PrefixSpanAlgorithm::partitionAllSequences() {
  const auto single_items = input_.uniqueSingleItems();
  // std::cout << "unique items: " << utils::print(single_items) << std::endl;

  std::map<Prefix, SequenceData> sequences;

  // prepare prefixes
  std::map<int, Prefix> prefixes;
  for (auto i : single_items) {
    final_sequences_.emplace_back(Prefix{i});
    prefixes[i] = Prefix{i};
  }

  // The input is devided into subsets. Each subset contains sequences
  // that begin with the given item.
  for (const Sequence &seq : input_) {
    if (seq.empty()) {
      continue;
    }
    for (auto pref : single_items) {
      for (auto it = seq.cbegin(); it != seq.cend(); ++it) {
        if (*it == pref && it + 1 != seq.cend()) {
          sequences[prefixes[pref]].emplace_back(it + 1, seq.end());
          break;
        }
      }
    }
  }

  return sequences;
}

void PrefixSpanAlgorithm::printSequences(
    const std::map<Prefix, SequenceData> &sequences) {
  for (const auto &sequence : sequences) {
    std::cout << "********************************************" << std::endl;
    std::cout << "sequence " << sequence.first[0] << std::endl;
    sequence.second.printData();
  }
  std::cout << "********************************************" << std::endl;
}

std::vector<int> PrefixSpanAlgorithm::frequentItems(
    const Prefix &prefix, const SequenceData &data) const {
  std::unordered_map<int, int> counter;

  // std::set<int> pref_set(prefix.begin(), prefix.end());

  for (const auto &seq : data) {
    if (seq.empty()) {
      continue;
    }

    std::set<int> items;

    // if prefix is a part of the first element
    auto it = seq.begin();
    while (it != seq.end() && *it != SEP) {
      items.insert(-1 * (*it));
      ++it;
    }

    // If item is negative, it means it's a part of the element that also
    // contains the prefix
    for (; it != seq.end(); ++it) {
      if (*it != SEP) {
        //  *(it - 1) == prefix.back()
        if (checkSeq(prefix, seq, it - seq.begin())) {
          items.insert(-1 * (*it));
        } else {
          items.insert(*it);
        }
      }
    }

    for (auto item : items) {
      ++counter[item];
    }
  }

  std::vector<int> infrequent_items;
  for (const auto &it : counter) {
    if (it.second > min_support_) {
      infrequent_items.push_back(it.first);
    }
  }
  return infrequent_items;
}

void PrefixSpanAlgorithm::recursiveSolve(const Prefix &prefix,
                                         const SequenceData &data) {
  // std::cout <<
  // "**************************************************************"
  //           << std::endl;
  std::cout << "recursiveSolve for " << utils::print(prefix) << std::endl;

  std::cout << "Projected data " << std::endl;
  data.printData();

  const auto freq_items = frequentItems(prefix, data);

  std::cout << "frequent items: " << utils::print(freq_items) << std::endl;

  if (freq_items.empty()) {
    //   std::cout << "no freq items, returning" << std::endl;
    return;
  }

  appendFinalSequences(prefix, freq_items);

  // For each item, construct a new prefix by appending the 'prefix' and create
  // a dataset that contains this prefix.
  for (auto pref : freq_items) {
    SequenceData projectedData;

    for (const Sequence &seq : data) {
      if (seq.empty()) {
        continue;
      }

      bool first_element{true};

      for (auto it = seq.cbegin(); it != seq.cend(); ++it) {
        if (*it == abs(pref) && it + 1 != seq.cend()) {
          // Additional conditions to cope with elements that contain multiple
          // items.

          // The first element cannot be positive, because if the first element
          // is a new itemset, sequence starts with SEP.
          if (it == seq.cbegin() && pref > 0) {
            continue;
          }

          if (it > seq.cbegin() && pref < 0 &&
              !checkSeq(prefix, seq, it - seq.begin())) {
            continue;
          }

          projectedData.emplace_back(it + 1, seq.cend());
          break;
        }
        if (*it == SEP) {
          first_element = false;
        }
      }
    }

    if (projectedData.size() <= min_support_) {
      continue;
    }

    auto new_prefix(prefix);
    if (pref > 0) {
      new_prefix.push_back(SEP);
      new_prefix.push_back(pref);
    } else {
      new_prefix.push_back(-1 * pref);
    }

    recursiveSolve(new_prefix, projectedData);
  }
}

void PrefixSpanAlgorithm::appendFinalSequences(const Prefix &prefix,
                                               const std::vector<int> &items) {
  std::lock_guard<std::mutex> lock(final_sequences_mutex_);
  for (auto item : items) {
    Sequence s(prefix);
    if (item > 0) {
      s.push_back(SEP);
      s.push_back(item);
    } else {
      s.push_back(-1 * item);
    }
    final_sequences_.push_back(s);
  }
}
