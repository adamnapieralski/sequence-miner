#pragma once

#include <InputData.h>

class Algorithm {
 public:
  virtual bool loadData(InputData input) = 0;
  virtual bool run() = 0;
};