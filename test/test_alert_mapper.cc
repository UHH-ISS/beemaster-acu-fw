//
// Created by florian on 12/21/16.
//

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