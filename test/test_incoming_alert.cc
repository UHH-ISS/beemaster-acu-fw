/* test_incoming_alert.cc
 * ACU Framework (test)
 *
 * Test whether the access to the message's fields works correctly.
 *
 * @author: 1wilkens
 */

#include <catch.hpp>

#include <acu/incoming_alert.h>

using namespace broker;

typedef std::chrono::duration<double> double_dur;

TEST_CASE("Testing IncomingAlert", "[incoming_alert]") {
    auto time_stamp = std::chrono::system_clock::now();
    auto truncated = std::chrono::duration_cast<double_dur>(time_stamp.time_since_epoch());
    auto broker_stamp = time_point{truncated.count()};

    // The (uint16_t) casts do not matter but are here for completeness
    auto topic = new std::string("some topic");
    auto rec = record({
            record::field(broker_stamp),
            record::field("127.0.0.1"),
            record::field((acu::port_t)8080),
            record::field("192.168.0.1"),
            record::field((acu::port_t)9090)
    });
    auto msg = message{"acu/test_event", rec};
    auto alert = acu::IncomingAlert(topic, msg);

    REQUIRE(alert.topic == topic);
    REQUIRE(*alert.topic == *topic);
    // Broker uses doubles internally to transmit time_points..
    // This loses us some precision and is the reason the following return value cannot be compared
    // with the original time_stamp. Instead we compare with the truncated version
    REQUIRE(std::chrono::duration_cast<double_dur>(alert.timestamp().time_since_epoch()) == truncated);
    REQUIRE(alert.source_ip() == "127.0.0.1");
    REQUIRE(alert.source_port() == 8080);
    REQUIRE(alert.destination_ip() == "192.168.0.1");
    REQUIRE(alert.destination_port() == 9090);

    std::cout << "ALERT: " << alert << std::endl;
}
