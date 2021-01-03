#include "PrefixSpanAlgorithm.h"

#include <algorithm>
#include <execution>
#include <sstream>
#include <unordered_map>
#include <chrono>

#include "SequenceData.h"
#include "Algorithm.h"
#include "utils.hpp"

namespace {
const Prefix null_prefix{0};
}

bool PrefixSpanAlgorithm::loadData(SequenceData input) {
  std::cout << "Load data for PrefixSpan algorithm" << std::endl;
  // input.printData();
  input_ = std::move(input);
  return true;
}

bool PrefixSpanAlgorithm::run(int min_support) {
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  std::cout << "Run PrefixSpan algorithm" << std::endl;

  min_support_ = min_support;

  input_.removeInfrequentItems(min_support_);
  // input_.printData();

  auto sequences = partitionAllSequences();

  // no longer needed
  input_.clear();

  std::for_each(
      std::execution::par, sequences.begin(), sequences.end(),
      [=](const auto &item) { recursiveSolve(item.first, item.second); });

  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "Execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

  printFinalSequences();

  return true;
}

std::map<Prefix, SequenceData> PrefixSpanAlgorithm::partitionAllSequences() {
  auto single_items = input_.uniqueSingleItems();
  std::cout << "unique items: " << utils::print(single_items) << std::endl;

  std::map<Prefix, SequenceData> sequences;

  for (auto item : single_items) {
    final_sequences_.push_back(Prefix{item});
  }

  // prepare prefixes
  std::map<int, Prefix> prefixes;
  for (auto i : single_items) {
    prefixes[i] = Prefix{i};
  }

  // The input is devided into subsets. Each subset contains sequences
  // that begin with the given item.
  for (Sequence &seq : input_) {
    if (seq.empty()) {
      continue;
    }
    for (auto pref : single_items) {
      for (auto it = seq.begin(); it != seq.end(); ++it) {
        if (*it == pref && it + 1 != seq.end()) {
          sequences[prefixes[pref]].push_back(Sequence(it + 1, seq.end()));
          break;
        }
      }
    }
  }

  return sequences;
}

void PrefixSpanAlgorithm::printSequences(
    const std::map<Prefix, SequenceData> &sequences) const {
  for (auto it = sequences.begin(); it != sequences.end(); ++it) {
    std::cout << "********************************************" << std::endl;
    std::cout << "sequence " << it->first[0] << std::endl;
    it->second.printData();
  }
  std::cout << "********************************************" << std::endl;
}

std::vector<int> PrefixSpanAlgorithm::frequentItems(
    const Prefix &prefix, const SequenceData &data) const {
  std::unordered_map<int, int> counter;

  for (const auto &seq : data) {
    if (seq.empty()) {
      continue;
    }

    std::set<int> items;

    if (seq.front() != -1) {
      items.insert(-1 * seq.front());
    }

    // If item is negative, it means it's a part of the element that also
    // contains the prefix
    for (auto it = seq.begin() + 1; it != seq.end(); ++it) {
      if (*it != -1) {
        if (*(it - 1) == prefix.back()) {
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
  for (auto it = counter.begin(); it != counter.end(); ++it) {
    if (it->second > min_support_) {
      infrequent_items.push_back(it->first);
    }
  }
  return infrequent_items;
}

void PrefixSpanAlgorithm::recursiveSolve(const Prefix &prefix,
                                         const SequenceData &data) {
  std::cout << "**************************************************************"
            << std::endl;
  std::cout << "recursiveSolve for " << utils::print(prefix) << std::endl;

  // std::cout << "Projected data " << std::endl;
  // data.printData();

  auto freq_items = frequentItems(prefix, data);

  std::cout << "frequent items: " << utils::print(freq_items) << std::endl;

  if (freq_items.empty()) {
    std::cout << "no freq items, returning" << std::endl;
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
      for (auto it = seq.begin(); it != seq.end(); ++it) {
        if (*it == abs(pref) && it + 1 != seq.end()) {
          // Additional conditions to cope with elements that contain multiple
          // items.

          if (it == seq.begin() && pref > 0) {
            continue;
          }

          if (it > seq.begin() && pref < 0 && prefix.size() > 0 &&
              *(it - 1) != prefix[prefix.size() - 1]) {
            continue;
          }

          projectedData.push_back(Sequence(it + 1, seq.end()));
          break;
        }
      }
    }

    if (projectedData.size() <= min_support_) {
      continue;
    }

    auto new_prefix(prefix);
    if (pref > 0) {
      new_prefix.push_back(-1);
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
      s.push_back(-1);
      s.push_back(item);
    } else {
      s.push_back(-1 * item);
    }
    final_sequences_.push_back(s);
  }
}