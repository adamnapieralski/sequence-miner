#include "IdList.h"

#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "utils.hpp"

IdList::IdList(const std::map<int, EidSequence>& data) {
  data_ = data;
}

IdList::IdList(int sid, int eid) {
  data_.insert(std::pair<int, EidSequence>(sid, EidSequence({ eid })));
}

void IdList::insert(const std::pair<int, EidSequence> seqEids) {
  data_.insert(seqEids);
}


void IdList::addEidToSeq(int sid, int eid) {
  if (data_.find(sid) != data_.end()) {
    data_[sid].push_back(eid);
  } else {
    data_.insert(std::pair<int, EidSequence>(sid, EidSequence{ eid }));
  }
}

void IdList::print() const {
  for (const auto& pair : data_) {
    std::cout << "SID: " << pair.first << "\t" << "EIDs: ";
    for (auto v : pair.second) {
      std::cout << v << " ";
    }
    std::cout << std::endl;
  }
}
