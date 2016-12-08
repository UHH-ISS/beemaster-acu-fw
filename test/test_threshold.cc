//
// Created by florian on 12/7/16.
//

#include "catch.hpp"

#include "acu/threshold.h"
#include <unordered_set>

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
    acu::Threshold t = acu::Threshold(2, "field", "value");
    // Stack allocation via aggregate initialization syntax
    acu::Threshold t2 = acu::Threshold{2, "field", "value"};
    // Heap allocation via `new`
    acu::Threshold *t3 = new acu::Threshold(2, "field", "value");

    // Test equality
    REQUIRE(t == t2);
    REQUIRE(t == *t3);
    REQUIRE(t2 == *t3);

    // Test inequality
    auto other = acu::Threshold{2, "test", "123"};
    REQUIRE_FALSE(t == other);
    REQUIRE(t != other);
}

TEST_CASE("Testing threshold hash and set", "[threshold]") {
    auto set = std::unordered_set<acu::Threshold>();

    // Default set is empty
    REQUIRE(set.empty());
    REQUIRE(set.size() == 0);

    auto t1 = acu::Threshold(1, "set", "test");
    set.insert(t1);

    // Now we should have one item
    REQUIRE(set.size() == 1);

    auto t2 = acu::Threshold(1, "set", "test");
    set.insert(t2);

    // t1 == t2 -> the set should still contain only one item
    REQUIRE(t1 == t2);
    REQUIRE(set.size() == 1);

    auto t3 = acu::Threshold(2, "sets", "tests");
    set.insert(t3);

    // t3 != t1/t2 -> the set should contain 2 items
    REQUIRE(t3 != t1);
    REQUIRE(t3 != t2);
    REQUIRE(set.size() == 2);
}
