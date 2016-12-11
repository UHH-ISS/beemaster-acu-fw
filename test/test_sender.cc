//
// Created by flx on 11.12.16.
//

#include <catch.hpp>
#include <acu/sender.h>
#include <broker/endpoint.hh>
#include <broker/message_queue.hh>
#include <broker/broker.hh>
#include <iostream>
#include <unistd.h>

// Provides a mock implementation for OutgoingAlert
class MockOutgoingAlert : public acu::OutgoingAlert {
    public: MockOutgoingAlert(std::string name, std::chrono::time_point<std::chrono::system_clock> timestamp)
                : acu::OutgoingAlert(name, timestamp) {};
};


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
    sleep(1); // sender is non blocking so we need to wait for the "listen" to take effect.
    bool success = sender->Send(mockAlert);

    REQUIRE(success);

    for (auto& msg : queue.want_pop()) {
        REQUIRE(msg.at(0) == alertTime.time_since_epoch().count());
        REQUIRE(msg.at(1) == alertName);
    }
}

TEST_CASE("Testing sender send failure", "[sender]") {
    // setup
    std::string local_ip = "127.0.0.1";
    std::uint16_t local_port = 9999;

    acu::Sender *sender = new acu::Sender(local_ip, local_port);

    REQUIRE(sender != NULL);

    auto alertTime = std::chrono::system_clock::now();
    MockOutgoingAlert *mockAlert = new MockOutgoingAlert("MyAlert", alertTime);

    // do test
    bool success = sender->Send(mockAlert);

    REQUIRE_FALSE(success);
}