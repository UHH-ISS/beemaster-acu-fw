/* test_correlation.cc
 * ACU Framework (test)
 *
 * Tests for basic assign behaviour.
 *
 * @author: 0ortmann
 */

#include "catch.hpp"

#include <acu/correlation.h>

TEST_CASE("Testing correlation pure abstract-class layout", "[correlation]") {
    REQUIRE(std::is_copy_assignable<acu::Correlation>());
    REQUIRE_FALSE(std::is_copy_constructible<acu::Correlation>());

    REQUIRE(std::is_move_assignable<acu::Correlation>());
    REQUIRE_FALSE(std::is_move_constructible<acu::Correlation>());

    REQUIRE_FALSE(std::is_standard_layout<acu::Correlation>());
    REQUIRE_FALSE(std::is_pod<acu::Correlation>());
}
