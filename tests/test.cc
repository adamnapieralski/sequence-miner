#define CATCH_CONFIG_MAIN

#include <memory>

#include <SpadeAlgorithm.h>

#include <catch.hpp>

TEST_CASE("SPADE Test") {
  auto spade = std::make_unique<SpadeAlgorithm>(false);
  REQUIRE(spade->loadData(
      SequenceData::load("path", ' ', char(), DataType::t_char, -1)));
  REQUIRE(spade->run(1));
}
