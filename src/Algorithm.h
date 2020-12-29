#pragma once

#include "SequenceData.h"

class Algorithm {
 public:
  virtual bool loadData(SequenceData input) = 0;
  virtual bool run(int min_support) = 0;

 protected:
  int min_support_{1};
  SequenceData input_;
};