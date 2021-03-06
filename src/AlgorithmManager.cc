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
  const auto& alg_type = parameters_.getString(par_algorithm);

  auto min_support = parameters_.getInt(par_min_support);
  auto spade_dfs = parameters_.getBool(par_spade_dfs);

  if (alg_type == "spade") {
    algorithm_ = std::make_unique<SpadeAlgorithm>(spade_dfs);
  } else if (alg_type == "prefixspan") {
    algorithm_ = std::make_unique<PrefixSpanAlgorithm>();
  } else {
    std::cout << "Wrong algorithm type";
    return 2;
  }

  // data loading configuration and processing
  try {
    auto path = parameters_.getString(par_input);
    auto sep = parameters_.getString(par_input_ids_separator);
    auto seq_sep = parameters_.getString(par_input_items_separator);
    auto limit = parameters_.getInt(par_input_limit);

    algorithm_->loadData(path, sep, seq_sep, limit);

  } catch (const std::runtime_error& e) {
    std::cout << "Error: " << e.what() << std::endl;
    return 1;
  }

  auto status = algorithm_->run(min_support);

  return status ? 0 : 3;
};

bool AlgorithmManager::exportResults() {
  if (!algorithm_) {
    return false;
  }

  try {
    auto outputFile = parameters_.getString(par_output_file);
    auto outputItemsSeparator = parameters_.getString(par_output_items_separator);
    auto outputEventsSeparator = parameters_.getString(par_output_events_separator);

    std::ofstream f(outputFile, std::ofstream::out);
    if (f) {
      algorithm_->exportFinalSequences(f, outputItemsSeparator, outputEventsSeparator);
      return true;
    }
  } catch (const std::runtime_error& e) {
    std::cout << "Error: " << e.what() << std::endl;
    return false;
  }
  return false;
}