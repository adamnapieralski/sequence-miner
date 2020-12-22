#include "AlgorithmManager.h"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <memory>

#include "InputData.h"
#include "PrefixSpanAlgorithm.h"
#include "SpadeAlgorithm.h"

namespace {

const std::string par_input = "input_file";
const std::string par_separator = "separator";
const std::string par_seq_items_separator = "seq_separator";
const std::string par_data_type = "data_type";
const std::string par_algorithm = "algorithm";
const std::string par_min_support = "min_support";

void readYamlNode(std::variant<int, char, std::string>& var,
                  const YAML::Node& n, DataType t) {
  if (n.IsScalar()) {
    switch (t) {
      case DataType::t_int:
        var = atoi(n.Scalar().c_str());
        break;
      case DataType::t_char:
        if (n.Scalar().size() == 1) {
          var = n.Scalar()[0];
        }
        break;
      case DataType::t_string:
        var = n.Scalar();
        break;
    }
  }
}

}  // namespace

AlgorithmManager::AlgorithmManager() {
  parameters_ = {{par_input, "./data/input.txt"},
                 {par_separator, ' '},
                 {par_seq_items_separator, char()},
                 {par_algorithm, "spade"},
                 {par_min_support, 10},
                 {par_data_type, "char"}};
}

void AlgorithmManager::loadConfig(const char* path) {
  YAML::Node config = YAML::LoadFile(path);

  readYamlNode(parameters_[par_input], config[par_input], DataType::t_string);
  readYamlNode(parameters_[par_separator], config[par_separator],
               DataType::t_char);
  readYamlNode(parameters_[par_seq_items_separator],
               config[par_seq_items_separator], DataType::t_char);
  readYamlNode(parameters_[par_algorithm], config[par_algorithm],
               DataType::t_string);
  readYamlNode(parameters_[par_min_support], config[par_min_support],
               DataType::t_int);
  readYamlNode(parameters_[par_data_type], config[par_data_type],
               DataType::t_string);
}

int AlgorithmManager::run() {
  auto algorithm = std::unique_ptr<Algorithm>();
  const auto& alg_type = std::get<std::string>(parameters_[par_algorithm]);

  if (alg_type == "spade") {
    algorithm = std::make_unique<SpadeAlgorithm>();
  } else if (alg_type == "prefixspan") {
    algorithm = std::make_unique<PrefixSpanAlgorithm>();
  } else {
    std::cout << "Wrong algorithm type";
    return 2;
  }

  try {
    const auto& path = std::get<std::string>(parameters_[par_input]);
    auto sep = std::get<char>(parameters_[par_separator]);
    auto seq_sep = std::get<char>(parameters_[par_seq_items_separator]);
    const auto& type = std::get<std::string>(parameters_[par_data_type]);
    auto dtype = type == "char" ? DataType::t_char : DataType::t_int;

    algorithm->loadData(InputData::load(path, sep, seq_sep, dtype));

  } catch (const std::runtime_error& e) {
    std::cout << "Error: " << e.what() << std::endl;
    return 1;
  }

  auto status = algorithm->run();

  return status ? 0 : 3;
};