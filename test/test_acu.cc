/*
 * Dataflow test for the ACU class.
 *
 * TODO: NOTE: This test works, because the hardcoded addresses inside the ACU are known
 * TODO: Note: Sender on 127.0.0.1:9998 and receiver on 127.0.0.1:9999 FIXME
 */

#include "catch.hpp"

#include <acu/acu.h>


TEST_CASE("Testing Acu class layout", "[acu]") {
    REQUIRE(std::is_copy_assignable<acu::Acu>());
    REQUIRE(std::is_copy_constructible<acu::Acu>());

    REQUIRE(std::is_move_assignable<acu::Acu>());
    REQUIRE(std::is_move_constructible<acu::Acu>());

    REQUIRE(std::is_standard_layout<acu::Acu>());
    REQUIRE_FALSE(std::is_pod<acu::Acu>());
}

TEST_CASE("Testing ACU roundtrip dataflow", "[Acu]") {

    // This is kind of a "framework integration test"

    // Mock an alerttype + topic
    // verify receiver is started
    // send sth, assert receive
    // mock mapping
    // assert mapping for correct topic -> alertType
    // mock aggregation
    // assert invocation of aggregation
    // (on pos agg test: assert invocation of correlation)
    // (on pos corr test: assert sender sending)

    FAIL();
}