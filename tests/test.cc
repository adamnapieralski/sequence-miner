#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <SpadeAlgorithm.hpp>

TEST_CASE("SPADE Test") {
  SpadeAlgorithm<int> spade;

  REQUIRE(spade.loadData({}));
  REQUIRE(spade.run());
}
