//
// Created by florian on 12/7/16.
//

#include "catch.hpp"

#include "acu/threshold.h"

TEST_CASE("Testing threshold class layout", "[threshold]") {
    REQUIRE(std::is_copy_assignable<acu::Threshold>());
    REQUIRE(std::is_copy_constructible<acu::Threshold>());
}

TEST_CASE("Testing threshold pod", "[threshold]") {
    REQUIRE(std::is_pod<acu::Threshold>());
}