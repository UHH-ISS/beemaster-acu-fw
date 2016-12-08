#include "catch.hpp"

#include "acu/logger.h"

using namespace acu;

TEST_CASE("Testing Logger::GetLogger", "[logger]") {
    auto name = "test";
    auto logger = Logger::GetLogger(name);

    REQUIRE( logger.Name == name );

    auto logger2 = Logger::GetLogger(name);

    // manual tests prove, it is the same object
    REQUIRE( logger.Name == logger2.Name );

    // TODO find a test for instance equality (i.e. logger == logger2 in memory).
    // TODO add more detailed tests.
    //      Would need to redirect output (wait for custom output impl).
}
