#pragma once

#include <map>
#include <string>
#include <variant>

using ParamsMap = std::map<std::string, std::variant<int, bool, char, std::string>>;

const std::string par_input = "input_file";
const std::string par_separator = "input_ids_separator";
const std::string par_seq_items_separator = "input_items_separator";
const std::string par_data_type = "data_type";
const std::string par_algorithm = "algorithm";
const std::string par_min_support = "min_support";
const std::string par_input_limit = "input_limit";
const std::string par_spade_dfs = "spade_dfs";
const std::string par_output_file = "output_file";
const std::string par_output_items_separator = "output_items_separator";

class Parameters {
 public:
  Parameters();
  void readConfig(const std::string &path);
  int getInt(const std::string &key);
  bool getBool(const std::string &key);
  char getChar(const std::string &key);
  std::string getString(const std::string &key);

 private:
  ParamsMap params_;
};
