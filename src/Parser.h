
#pragma once

#include <fstream>
#include <unordered_map>
#include <vector>

using SequenceMap = std::unordered_map<int, std::vector<int>>;

namespace parser {

SequenceMap readCharDataNoSep(std::ifstream& f, char sep);
SequenceMap readSpfm(std::ifstream& f, int limit);

}  // namespace parser