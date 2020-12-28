#pragma once

#include "Parameters.h"

class AlgorithmManager {
 public:
  AlgorithmManager() = default;
  void loadConfig(const char* path);
  int run();

 private:
  Parameters parameters_;
};