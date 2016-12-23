/* test_alert_mapper.cc
 * ACU Framework (test)
 *
 * Tests for basic assign behaviour.
 *
 * @author: 1wilkens
 */

#include "catch.hpp"
#include <acu/alert_mapper.h>

TEST_CASE("Testing AlertMapper abstract class layout", "[alert_mapper]") {
    REQUIRE(std::is_copy_assignable<acu::AlertMapper>());
    REQUIRE(std::is_copy_constructible<acu::AlertMapper>());

    REQUIRE(std::is_move_assignable<acu::AlertMapper>());
    REQUIRE(std::is_move_constructible<acu::AlertMapper>());

    REQUIRE_FALSE(std::is_standard_layout<acu::AlertMapper>());
    REQUIRE_FALSE(std::is_pod<acu::AlertMapper>());
}

// TODO What about checking the outcoming alert? Whether it is correctly moved
//      and not copied etc.
