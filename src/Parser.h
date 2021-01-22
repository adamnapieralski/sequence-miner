
#pragma once

#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using SequenceMap = std::unordered_map<int, std::vector<int>>;

namespace parser {

SequenceMap readStringData(std::ifstream &f, std::string idSep, std::string elemSep);
SequenceMap readSpfm(std::ifstream& f, int limit);

}  // namespace parser