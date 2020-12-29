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

IdList::IdList(const std::map<int, EidSequence>& idList) {
    data_ = idList;
}