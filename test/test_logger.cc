#include "catch.hpp"

#include "acu/logger.h"

using namespace acu;

TEST_CASE("Testing Logger::GetLogger", "[logger]") {
    auto name = "test";
    auto logger = Logger::GetLogger(name);
    REQUIRE( logger.Name == name );

    // TODO add more detailed tests.
    //      Would need to redirect output (wait for custom output impl).
}
