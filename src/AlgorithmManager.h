#pragma once

#include <memory>

#include "Algorithm.h"
#include "Parameters.h"

class AlgorithmManager {
 public:
  AlgorithmManager() = default;
  void loadConfig(const char* path);
  int run();
  bool exportResults(const char* path);

 private:
  std::unique_ptr<Algorithm> algorithm_{};
  Parameters parameters_;
};