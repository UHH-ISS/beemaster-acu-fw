/* test_incoming_alert.cc
 * ACU Framework (test)
 *
 * Test whether the access to the message's fields works correctly.
 *
 * @author: 1wilkens
 */

#include <catch.hpp>
#include <iostream>
#include <broker/message.hh>
#include "acu/incoming_alert.h"

TEST_CASE("Testing IncomingAlert", "[incoming_alert]") {
    auto time_stamp = std::chrono::system_clock::now();
    auto val = std::chrono::duration_cast<std::chrono::duration<double>>(time_stamp.time_since_epoch());
    auto broker_stamp = broker::time_point{val.count()};

    // The (uint16_t) casts do not matter but are here for completeness
    auto topic = new std::string("some topic");
    auto msg = broker::message{broker_stamp, "incident", "proto", "127.0.0.1", (uint16_t)8080, "192.168.0.1", (uint16_t)9090};
    auto alert = acu::IncomingAlert(topic, msg);

    // TODO: the following test is incomplete, this requires final implementation of the timestamp method.
    // TODO: Change to plain REQUIRE when fixed.
    REQUIRE(alert.topic == topic);
    REQUIRE(*alert.topic == *topic);
    REQUIRE_FALSE(alert.timestamp() == time_stamp);
    REQUIRE(alert.incident_type() == "incident");
    REQUIRE(alert.protocol() == "proto");
    REQUIRE(alert.source_ip() == "127.0.0.1");
    REQUIRE(alert.source_port() == 8080);
    REQUIRE(alert.destination_ip() == "192.168.0.1");
    REQUIRE(alert.destination_port() == 9090);
}
