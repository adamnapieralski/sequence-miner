
#pragma once

#include <fstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>

using SequenceMap = std::unordered_map<int, std::vector<int>>;

namespace parser {

SequenceMap readStringData(std::ifstream &f, std::string idSep, std::string elemSep,
                           std::map<std::string, int>& stringToInt,
                           std::map<int, std::string>& intToString);
SequenceMap readSpfm(std::ifstream& f, int limit);

}  // namespace parser