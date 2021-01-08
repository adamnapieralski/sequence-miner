#include "Parser.h"

#include <algorithm>
#include <iostream>
#include <map>

#include "utils.hpp"

SequenceMap parser::readCharDataNoSep(std::ifstream &f, char sep) {
  int seq_id = 0;
  int time_id = 0;

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

    auto &seq_item = seqs[seq_id];
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

  std::cout << "Char to Int map: " << utils::printMap(char_to_int) << std::endl;

  return seqs;
}

SequenceMap parser::readSpfm(std::ifstream &f, int limit) {
  SequenceMap seqs;

  std::string line;

  int seq_id = 0;

  if (limit == -1) {
    limit = std::numeric_limits<int>::max();
  }

  while (!f.eof() && --limit >= 0) {
    getline(f, line);

    std::istringstream iss(line);

    int val = 0;
    std::vector<int> v;

    while (iss >> val && val != -2) {
      v.push_back(val);
    }

    seqs[++seq_id] = v;
  }

  return seqs;
}
