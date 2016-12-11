//
// Created by flx on 09.12.16.
//

#include "catch.hpp"

#include <acu/aggregation.h>

TEST_CASE("Testing aggregation pure abstract-class layout", "[aggregation]") {
    REQUIRE(std::is_copy_assignable<acu::Aggregation>());
    REQUIRE_FALSE(std::is_copy_constructible<acu::Aggregation>());

    REQUIRE(std::is_move_assignable<acu::Aggregation>());
    REQUIRE_FALSE(std::is_move_constructible<acu::Aggregation>());

    REQUIRE_FALSE(std::is_standard_layout<acu::Aggregation>());
    REQUIRE_FALSE(std::is_pod<acu::Aggregation>());
}