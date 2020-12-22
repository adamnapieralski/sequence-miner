#include "InputData.h"

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

using SequenceMap = std::unordered_map<int, std::vector<int>>;
using Sequence = std::vector<int>;

namespace {
SequenceMap readCharDataNoSep(std::ifstream& f, char sep) {
  int seq_id;
  int time_id;

  SequenceMap seqs;

  std::map<char, int> char_to_int;
  auto max_int = 0;

  std::string line;
  while (!f.eof()) {
    getline(f, line);
    std::replace(line.begin(), line.end(), sep, ' ');
    std::stringstream ss(line);
    ss >> seq_id;
    ss >> time_id;

    std::string seq;
    ss >> seq;

    auto& seq_item = seqs[seq_id];
    for (auto s : seq) {
      auto it = char_to_int.find(s);
      if (it == char_to_int.end()) {
        char_to_int[s] = ++max_int;
        seq_item.push_back(max_int);
      } else {
        seq_item.push_back(it->second);
      }
    }
    if (!seq.empty()) {
      seq_item.push_back(-1);
    }
  }

  return seqs;
}

std::vector<Sequence> mapValues(SequenceMap& map) {
  std::vector<Sequence> v;
  for (auto it = map.begin(); it != map.end(); ++it) {
    v.emplace_back(std::move(it->second));
  }
  return v;
}

}  // namespace

InputData InputData::load(const std::string& input, char separator,
                          char seq_separator, DataType data_type) {
  std::cout << "Loading file " << input << std::endl;

  std::ifstream input_file{input};

  if (!input_file) {
    throw std::runtime_error("Input file not found");
  }

  SequenceMap seqs;

  if (seq_separator == char() and data_type == DataType::t_char) {
    seqs = readCharDataNoSep(input_file, separator);
  }

  InputData d;
  d.data_ = mapValues(seqs);

  std::cout << "Loaded " << d.data_.size() << " sequences" << std::endl;

  for (const auto& v : d.data_) {
    std::cout << "seq: ";
    for (auto s : v) {
      std::cout << s << " ";
    }
    std::cout << std::endl;
  }

  return d;
}

int InputData::size() const { return static_cast<int>(data_.size()); }

const std::vector<int>& InputData::sequence(int id) const { return data_[id]; };

iterator InputData::begin() { return data_.begin(); }
iterator InputData::end() { return data_.end(); }

const_iterator InputData::begin() const { return data_.begin(); }
const_iterator InputData::end() const { return data_.end(); }