#define CATCH_CONFIG_MAIN

#include <SpadeAlgorithm.h>

#include <catch.hpp>

TEST_CASE("SPADE Test") {
  SpadeAlgorithm spade;

  REQUIRE(spade.loadData(
      SequenceData::load("path", ' ', char(), DataType::t_char, -1)));
  REQUIRE(spade.run(2));
}
