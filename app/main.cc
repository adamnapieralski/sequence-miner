
#include <AlgorithmManager.h>

#include <iostream>

int main(int argc, const char* argv[]) {
  AlgorithmManager m;

  auto config = argc == 2 ? argv[1] : "";
  m.loadConfig(config);

  return m.run();
}
