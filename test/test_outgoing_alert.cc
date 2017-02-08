/* test_outgoing_alert.cc
 * ACU Framework (test)
 *
 * Tests for basic assign behaviour and the output to broker message format.
 *
 * @author: 0ortmann
 */

#include <acu/outgoing_alert.h>
#include "catch.hpp"

#include "acu/threshold.h"

TEST_CASE("Testing OutgoingAlert class layout", "[outgoing_alert]") {
    REQUIRE(std::is_copy_assignable<acu::OutgoingAlert>());
    REQUIRE(std::is_copy_constructible<acu::OutgoingAlert>());

    REQUIRE(std::is_move_assignable<acu::OutgoingAlert>());
    REQUIRE(std::is_move_constructible<acu::OutgoingAlert>());

    REQUIRE_FALSE(std::is_standard_layout<acu::OutgoingAlert>());
    REQUIRE_FALSE(std::is_pod<acu::OutgoingAlert>());
}

TEST_CASE("Testing OutgoingAlert ToMessage default behavior", "[outgoing_alert]") {
    auto now = std::chrono::system_clock::now();
    acu::OutgoingAlert* alert = new acu::OutgoingAlert("BAM", now);

    REQUIRE(alert->incidentName == "BAM");
    REQUIRE(alert->timestamp == now);

    auto msg = alert->ToMessage();

    REQUIRE(msg[0].value == alert->EventName());
    REQUIRE(msg[1].value == alert->timestamp.time_since_epoch().count());
    REQUIRE(msg[2].value == alert->incidentName);
}
