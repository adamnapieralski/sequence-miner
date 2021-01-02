#include "AlgorithmManager.h"

#include <fstream>
#include <memory>

#include "PrefixSpanAlgorithm.h"
#include "SequenceData.h"
#include "SpadeAlgorithm.h"

void AlgorithmManager::loadConfig(const char* path) {
  parameters_.readConfig(path);
}

int AlgorithmManager::run() {
  auto algorithm = std::unique_ptr<Algorithm>();
  const auto& alg_type = parameters_.getString(par_algorithm);

  if (alg_type == "spade") {
    algorithm = std::make_unique<SpadeAlgorithm>();
  } else if (alg_type == "prefixspan") {
    algorithm = std::make_unique<PrefixSpanAlgorithm>();
  } else {
    std::cout << "Wrong algorithm type";
    return 2;
  }

  try {
    auto path = parameters_.getString(par_input);
    auto sep = parameters_.getChar(par_separator);
    auto seq_sep = parameters_.getChar(par_seq_items_separator);
    auto type = parameters_.getString(par_data_type);
    auto dtype = type == "char" ? DataType::t_char : DataType::t_int;

    algorithm->loadData(SequenceData::load(path, sep, seq_sep, dtype));

  } catch (const std::runtime_error& e) {
    std::cout << "Error: " << e.what() << std::endl;
    return 1;
  }

  auto min_support = parameters_.getInt(par_min_support);

  auto status = algorithm->run(min_support);

  return status ? 0 : 3;
};
