#include <SpadeAlgorithm.hpp>
#include <iostream>

int main(int argc, const char* argv[]) {
  SpadeAlgorithm<int> spade;

  spade.loadData({});
  spade.run();

  return 0;
}
