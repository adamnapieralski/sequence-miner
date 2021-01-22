#include "Parser.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>

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

SequenceMap parser::readStringData(std::ifstream &f, std::string idSep, std::string elemSep) {
  int seq_id = 0;
  int time_id = 0;
  int sepPos = 0;

  SequenceMap seqs;

  std::map<std::string, int> string_to_int;
  auto max_int = 0;

  std::string line;
  while (!f.eof()) {
    getline(f, line);
    if (line.length() == 0) break;

    // read sid
    sepPos = line.find(idSep);
    std::cout << line.substr(0, sepPos) << std::endl;
    seq_id = std::stoi(line.substr(0, sepPos));
    line.erase(0, sepPos + idSep.length());

    // read tid
    sepPos = line.find(idSep);
    std::cout << line.substr(0, sepPos) << std::endl;
    time_id = std::stoi(line.substr(0, sepPos));
    line.erase(0, sepPos + idSep.length());

    auto &seq_item = seqs[seq_id];

    while (line.size() > 0) {
      sepPos = line.find(elemSep);
      std::string elem = "";
      if (sepPos != std::string::npos) {
        elem = line.substr(0, sepPos);
        line.erase(0, sepPos + elemSep.length());
      } else {
        // only \n, may add \r for Windows
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
        elem = line;
        line = "";
      }
      auto it = string_to_int.find(elem);
      if (it == string_to_int.end()) {
        string_to_int[elem] = ++max_int;
        seq_item.push_back(max_int);
      } else {
        seq_item.push_back(it->second);
      }
    }

    seq_item.push_back(-1);
  }

  std::cout << "String to Int map: " << utils::printMap(string_to_int) << std::endl;
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
