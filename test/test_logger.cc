/* test_logger.cc
 * ACU Framework (test)
 *
 * Tries to test the logger (which is currently broken).
 *
 * @author: 1jost, 1wilkens
 */
#include "catch.hpp"

#include "acu/logger.h"

using namespace acu;

TEST_CASE("Testing Logger::GetLogger", "[logger]") {
    auto name = "test";
    auto logger = Logger::GetLogger(name);

    REQUIRE(logger->name == name);

    auto logger2 = Logger::GetLogger(name);

    REQUIRE(logger->name == logger2->name);
    REQUIRE(logger == logger2);

    // This is here to prevent accidental deletion of
    // Logger::RecursiveArgs(T arg, Args... args) as it is marked unused
    logger2->Debug("IGNORE", "ME");

    // TODO add more detailed tests.
    //      Would need to redirect output (wait for custom output impl).
}
