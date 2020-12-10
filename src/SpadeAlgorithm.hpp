#pragma once

#include <iostream>

#include "Algorithm.h"

template <typename T>
class SpadeAlgorithm : public Algorithm<T> {
 public:
  SpadeAlgorithm() = default;
  bool loadData(const std::vector<std::vector<T>> &sequences) override {
    std::cout << "Load data for SPADE algorithm" << std::endl;
    return true;
  }

  bool run() override {
    std::cout << "Run SPADE algorithm" << std::endl;
    return true;
  }
};