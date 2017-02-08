/* test_sender.cc
 * ACU Framework (test)
 *
 * Intensive testing of the senders functionality.
 *
 * @author: 0ortmann
 */

#include <catch.hpp>
#include <acu/sender.h>
#include <broker/message_queue.hh>
#include <iostream>
#include <unistd.h>
#include <broker/broker.hh>

TEST_CASE("Testing sender class layout", "[sender]") {
    REQUIRE(std::is_copy_assignable<acu::Sender>());
    REQUIRE(std::is_copy_constructible<acu::Sender>());

    REQUIRE(std::is_move_assignable<acu::Sender>());
    REQUIRE(std::is_move_constructible<acu::Sender>());

    REQUIRE(std::is_standard_layout<acu::Sender>());
    REQUIRE_FALSE(std::is_pod<acu::Sender>());
}

TEST_CASE("Testing sender send functionality", "[sender]") {
    // setup
    broker::init();

    auto alertName = "MyAlert";
    auto alertTime = std::chrono::system_clock::now();
    auto *mockAlert = new acu::OutgoingAlert(alertName, alertTime);

    SECTION("Testing successful send") {
        std::string local_ip = "127.0.0.1";
        acu::port_t local_port = 9970;

        // remote bro-broker "mock" via localhost
        broker::endpoint rec_ep("Receiver Endpoint",
                                broker::AUTO_ROUTING | broker::AUTO_ADVERTISE);
        broker::message_queue queue(alertName, rec_ep, broker::GLOBAL_SCOPE);

        bool listening = rec_ep.listen(local_port, local_ip.c_str());
        REQUIRE(listening);

        usleep(300 * 1000); // wait for listen to grip...

        acu::Sender *sender = new acu::Sender(local_ip, local_port);
        REQUIRE(sender != nullptr);

        // wait on this site with the mocked receiver:
        REQUIRE(rec_ep.incoming_connection_status().need_pop().front().status
                == broker::incoming_connection_status::tag::established);

        // do test
        usleep(100 * 1000);
        bool success = sender->Send(mockAlert);
        REQUIRE(success);

        // the non-blocking is wanted here. test should break if nothing is there instead of waiting forever.
        for (auto &msg : queue.want_pop()) {
            REQUIRE(msg.at(0) == alertTime.time_since_epoch().count());
            REQUIRE(msg.at(1) == alertName);
        }
    }

    SECTION("Testing sending without peering") {
        // provide invalid address...
        acu::Sender *sender = new acu::Sender("127.0.0.1", 1234);

        // do test
        bool success = sender->Send(mockAlert);

        // not sent? -> should not have been converted!
        REQUIRE_FALSE(success);
    }
}
