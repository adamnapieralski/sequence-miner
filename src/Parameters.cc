#include "Parameters.h"

#include <yaml-cpp/yaml.h>

namespace {

enum class ParamDataType { t_int, t_char, t_string };

void readYamlNode(std::variant<int, char, std::string> &var,
                  const YAML::Node &n, ParamDataType t) {
  if (n.IsScalar()) {
    switch (t) {
      case ParamDataType::t_int:
        var = atoi(n.Scalar().c_str());
        break;
      case ParamDataType::t_char:
        if (n.Scalar().size() == 1) {
          var = n.Scalar()[0];
        }
        break;
      case ParamDataType::t_string:
        var = n.Scalar();
        break;
    }
  }
}

}  // namespace

Parameters::Parameters() {
  params_ = {{par_input, "./data/input.txt"},
             {par_separator, ' '},
             {par_seq_items_separator, char()},
             {par_algorithm, "spade"},
             {par_min_support, 2},
             {par_data_type, "char"}};
}

void Parameters::readConfig(const std::string &path) {
  YAML::Node config = YAML::LoadFile(path);

  readYamlNode(params_[par_input], config[par_input], ParamDataType::t_string);
  readYamlNode(params_[par_separator], config[par_separator],
               ParamDataType::t_char);
  readYamlNode(params_[par_seq_items_separator],
               config[par_seq_items_separator], ParamDataType::t_char);
  readYamlNode(params_[par_algorithm], config[par_algorithm],
               ParamDataType::t_string);
  readYamlNode(params_[par_min_support], config[par_min_support],
               ParamDataType::t_int);
  readYamlNode(params_[par_data_type], config[par_data_type],
               ParamDataType::t_string);
}

int Parameters::getInt(const std::string &key) {
  return std::get<int>(params_[key]);
}

char Parameters::getChar(const std::string &key) {
  return std::get<char>(params_[key]);
}

std::string Parameters::getString(const std::string &key) {
  return std::get<std::string>(params_[key]);
}
