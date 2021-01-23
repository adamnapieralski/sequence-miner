#include "Parameters.h"

#include <yaml-cpp/yaml.h>

namespace {

enum class ParamDataType { t_int, t_bool, t_char, t_string };

void readYamlNode(std::variant<int, bool, char, std::string> &var,
                  const YAML::Node &n, ParamDataType t) {
  if (n.IsScalar()) {
    switch (t) {
      case ParamDataType::t_int:
        var = atoi(n.Scalar().c_str());
        break;
      case ParamDataType::t_bool:
        if (n.Scalar() == "true") {
          var = true;
        } else if (n.Scalar() == "false") {
          var = false;
        }
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
             {par_data_type, "string"},
             {par_spade_dfs, false},
             {par_input_limit, -1},
             {par_output_file, "./data/output.txt"},
             {par_output_items_separator, ","}};
}

void Parameters::readConfig(const std::string &path) {
  YAML::Node config = YAML::LoadFile(path);

  readYamlNode(params_[par_input], config[par_input], ParamDataType::t_string);
  readYamlNode(params_[par_separator], config[par_separator],
               ParamDataType::t_string);
  readYamlNode(params_[par_seq_items_separator],
               config[par_seq_items_separator], ParamDataType::t_string);
  readYamlNode(params_[par_algorithm], config[par_algorithm],
               ParamDataType::t_string);
  readYamlNode(params_[par_min_support], config[par_min_support],
               ParamDataType::t_int);
  readYamlNode(params_[par_data_type], config[par_data_type],
               ParamDataType::t_string);
  readYamlNode(params_[par_input_limit], config[par_input_limit],
               ParamDataType::t_int);
  readYamlNode(params_[par_spade_dfs], config[par_spade_dfs],
               ParamDataType::t_bool);
  readYamlNode(params_[par_output_file], config[par_output_file],
               ParamDataType::t_string);
  readYamlNode(params_[par_output_items_separator], config[par_output_items_separator],
               ParamDataType::t_string);
}

int Parameters::getInt(const std::string &key) {
  return std::get<int>(params_[key]);
}

bool Parameters::getBool(const std::string &key) {
  return std::get<bool>(params_[key]);
}

char Parameters::getChar(const std::string &key) {
  return std::get<char>(params_[key]);
}

std::string Parameters::getString(const std::string &key) {
  return std::get<std::string>(params_[key]);
}
