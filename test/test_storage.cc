/* test_storage.cc
 * ACU Framework (test)
 * 
 * Test default assign behaviour.
 *
 * @author: 0ortmann
 */

#include "catch.hpp"

#include <acu/storage.h>

TEST_CASE("Testing storage pure abstract-class layout", "[storage]") {
    REQUIRE(std::is_copy_assignable<acu::Storage>());
    REQUIRE_FALSE(std::is_copy_constructible<acu::Storage>());

    REQUIRE(std::is_move_assignable<acu::Storage>());
    REQUIRE_FALSE(std::is_move_constructible<acu::Storage>());

    REQUIRE_FALSE(std::is_standard_layout<acu::Storage>());
    REQUIRE_FALSE(std::is_pod<acu::Storage>());
}
