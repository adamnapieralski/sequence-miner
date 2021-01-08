#pragma once

#include <map>
#include <string>
#include <variant>

using ParamsMap = std::map<std::string, std::variant<int, char, std::string>>;

const std::string par_input = "input_file";
const std::string par_separator = "separator";
const std::string par_seq_items_separator = "seq_separator";
const std::string par_data_type = "data_type";
const std::string par_algorithm = "algorithm";
const std::string par_min_support = "min_support";
const std::string par_input_limit = "input_limit";

class Parameters {
 public:
  Parameters();
  void readConfig(const std::string &path);
  int getInt(const std::string &key);
  char getChar(const std::string &key);
  std::string getString(const std::string &key);

 private:
  ParamsMap params_;
};
