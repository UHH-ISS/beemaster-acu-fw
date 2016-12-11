//
// Created by flx on 11.12.16.
//

#include <catch.hpp>
#include <acu/sender.h>
#include <broker/endpoint.hh>
#include <broker/message_queue.hh>
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
    std::string local_ip = "127.0.0.1";
    std::uint16_t local_port = 9999;

    acu::Sender *sender = new acu::Sender(local_ip, local_port);

    REQUIRE(sender != NULL);

    // mock
    class MockOutgoingAlert : public acu::OutgoingAlert {
    public: MockOutgoingAlert(std::string name, std::chrono::time_point<std::chrono::system_clock> timestamp)
            : acu::OutgoingAlert(name, timestamp) {};
    };

    auto alertName = "MyAlert";
    auto alertTime = std::chrono::system_clock::now();
    MockOutgoingAlert *mockAlert = new MockOutgoingAlert(alertName, alertTime);

    // broker mock
    broker::init();
    broker::endpoint rec_ep("Receiver Endpoint");
    bool listening = rec_ep.listen(local_port, local_ip.c_str());
    REQUIRE(listening);
    broker::message_queue queue(alertName, rec_ep);

    // do test
    sender->Send(mockAlert);

    for (auto& msg : queue.need_pop()) {
        REQUIRE(msg.at(0) == alertTime.time_since_epoch().count());
        REQUIRE(msg.at(1) == alertName);
    }
}