
#include <AlgorithmManager.h>

#include <iostream>

int main(int argc, const char* argv[]) {
  AlgorithmManager m;

  const auto *config = argc > 1 ? argv[1] : "data/config.yaml";
  m.loadConfig(config);

  auto status = m.run();

  if (status == 0) {
    m.exportResults();
  }

  return status;
}
