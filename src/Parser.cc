#include "Parser.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include "SequenceData.h"
#include "utils.hpp"

SequenceMap parser::readStringData(std::ifstream& f, std::string idSep,
                                   std::string elemSep,
                                   std::map<std::string, int>& stringToInt,
                                   std::map<int, std::string>& intToString) {
  int seqId = 0;
  int timeId = 0;
  int sepPos = 0;

  SequenceMap seqs;

  auto maxInt = 0;

  std::string line;
  while (!f.eof()) {
    getline(f, line);
    if (line.length() == 0) break;

    // read sid
    sepPos = line.find(idSep);
    seqId = std::stoi(line.substr(0, sepPos));
    line.erase(0, sepPos + idSep.length());

    // read tid
    sepPos = line.find(idSep);
    timeId = std::stoi(line.substr(0, sepPos));
    line.erase(0, sepPos + idSep.length());

    auto& seq_item = seqs[seqId];

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
      auto it = stringToInt.find(elem);
      if (it == stringToInt.end()) {
        stringToInt[elem] = ++maxInt;
        intToString[maxInt] = elem;
        seq_item.push_back(maxInt);
      } else {
        seq_item.push_back(it->second);
      }
    }

    seq_item.push_back(SEP);
  }

  return seqs;
}

SequenceMap parser::readSpfm(std::ifstream& f, int limit) {
  SequenceMap seqs;

  std::string line;

  int seqId = 0;

  if (limit == -1) {
    limit = std::numeric_limits<int>::max();
  }

  while (!f.eof() && --limit >= 0) {
    getline(f, line);

    std::istringstream iss(line);

    int val = 0;
    std::vector<int> v;

    while (iss >> val && val != -2) {
      if (val == -1) {
        v.push_back(SEP);
      } else {
        v.push_back(val);
      }
    }

    seqs[++seqId] = v;
  }

  return seqs;
}
