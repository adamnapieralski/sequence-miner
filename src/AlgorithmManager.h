#pragma once

#include <map>
#include <string>
#include <variant>
#include <vector>

using Parameters = std::map<std::string, std::variant<int, char, std::string>>;

class AlgorithmManager {
 public:
  AlgorithmManager();
  void loadConfig(const char* path);
  int run();

 private:
  Parameters parameters_;
};