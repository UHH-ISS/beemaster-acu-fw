//
// Created by flx on 11.12.16.
//

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

TEST_CASE("Testing OutgoingAlert AsMessage default behavior", "[outgoing_alert]") {
    auto now = std::chrono::system_clock::now();
    acu::OutgoingAlert* alert = new acu::OutgoingAlert("BAM", now);

    REQUIRE(alert->incidentName == "BAM");
    REQUIRE(alert->timestamp == now);

    auto msg = alert->AsMessage();

    REQUIRE(msg[0].value == alert->timestamp.time_since_epoch().count());
    REQUIRE(msg[1].value == alert->incidentName);
}