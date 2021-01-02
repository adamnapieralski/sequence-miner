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
    data_[sid].insert(eid);
  } else {
    data_.insert(std::pair<int, EidSequence>(sid, EidSequence{ eid }));
  }
}

std::map<int, EidSequence> IdList::getData() const {
  return data_;
}


IdList IdList::joinEqual(const IdList& idList) const {
  IdList resIdList;
  for (const auto& seqEid : idList.data_) {
    int sid = seqEid.first;
    if (data_.find(sid) != data_.end()) {
      EidSequence eidSeq;
      std::set_intersection(
        seqEid.second.cbegin(), seqEid.second.cend(),
        data_.at(sid).cbegin(), data_.at(sid).cend(),
        std::inserter(eidSeq, eidSeq.begin())
      );
      if (!eidSeq.empty()) {
        resIdList.insert(std::pair<int, EidSequence>(sid, eidSeq));
      }
    }
  }
  return resIdList;
}

IdList IdList::joinLatter(const IdList& idList) const {
  IdList resIdList;
  for (const auto& seqEid : idList.data_) {
    int sid = seqEid.first;
    if (data_.find(sid) != data_.end()) {
      int minLeft = *(data_.at(sid).begin());
      auto upBoundIt = seqEid.second.upper_bound(minLeft);
      EidSequence eidSeq(upBoundIt, seqEid.second.end());
      if (!eidSeq.empty()) {
        resIdList.insert(std::pair<int, EidSequence>(sid, eidSeq));
      }
    }
  }
  return resIdList;
}

int IdList::size() const {
  return data_.size();
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