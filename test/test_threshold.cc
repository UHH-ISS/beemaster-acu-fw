//
// Created by florian on 12/7/16.
//

#include "catch.hpp"

#include "acu/threshold.h"

TEST_CASE("Testing threshold class layout", "[threshold]") {
    REQUIRE(std::is_copy_assignable<acu::Threshold>());
    REQUIRE(std::is_copy_constructible<acu::Threshold>());

    REQUIRE(std::is_move_assignable<acu::Threshold>());
    REQUIRE(std::is_move_constructible<acu::Threshold>());

    REQUIRE(std::is_standard_layout<acu::Threshold>());
    REQUIRE_FALSE(std::is_pod<acu::Threshold>());
}

TEST_CASE("Testing threshold equality", "[threshold]") {
    // Stack allocation via regular ctor call
    acu::Threshold val = acu::Threshold();
    val.count = 2;
    val.field_name = "field";
    val.value = "value";
    // Stack allocation via aggregate initialization syntax
    acu::Threshold val2 = acu::Threshold{2, "field", "value"};
    // Heap allocation via `new`
    acu::Threshold *val3 = new acu::Threshold();
    val3->count = 2;
    val3->field_name = "field";
    val3->value = "value";

    // Test equality
    REQUIRE(val == val2);
    REQUIRE(val == *val3);
    REQUIRE(val2 == *val3);

    // Test inequality
    auto other = acu::Threshold{2, "test", "123"};
    REQUIRE_FALSE(val == other);
    REQUIRE(val != other);
}