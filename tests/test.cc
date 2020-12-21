#define CATCH_CONFIG_MAIN

#include <SpadeAlgorithm.h>

#include <catch.hpp>

TEST_CASE("SPADE Test") {
  SpadeAlgorithm spade;

  REQUIRE(spade.loadData(InputData::load("path", ' ')));
  REQUIRE(spade.run());
}
