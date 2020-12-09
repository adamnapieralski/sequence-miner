#pragma once
#include <vector>

template <typename T>
class Algorithm {
public:
    virtual bool loadData(const std::vector<std::vector<T>> &sequences) = 0;
    virtual bool run() = 0;
};