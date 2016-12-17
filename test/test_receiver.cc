//
// Created by florian on 12/16/16.
//

#include "catch.hpp"

#include <acu/receiver.h>

#include <unistd.h>
#include <iostream>

TEST_CASE("Testing Receiver class layout", "[receiver]") {
    REQUIRE(std::is_copy_assignable<acu::Receiver>());
    REQUIRE(std::is_copy_constructible<acu::Receiver>());

    REQUIRE(std::is_move_assignable<acu::Receiver>());
    REQUIRE(std::is_move_constructible<acu::Receiver>());

    REQUIRE(std::is_standard_layout<acu::Receiver>());
    REQUIRE_FALSE(std::is_pod<acu::Receiver>());
}

TEST_CASE("Testing Receiver receive", "[receiver]") {
    // Setup
    auto topics = std::vector<std::string>{
        "topic1",
        "topic2",
        "topic3"
    };
    std::string local_ip = "127.0.0.1";
    acu::port_t local_port = 9980;
    auto rec = acu::Receiver(local_ip, local_port, &topics);

    auto called = std::map<std::string, broker::message>();
    auto cb = [&](const std::string topic, const broker::message &msg) {
        std::cout << "called callback in test for topic " + topic << std::endl;
        called.emplace(topic, msg);
    };

    SECTION("Testing Receiver receive single queue success") {

        REQUIRE(called.size() == 0);

        rec.Listen(cb);
        usleep(500 * 1000);

        auto msg = broker::message{"testmessage"};
        auto sender = broker::endpoint("sender");
        sender.peer(local_ip, local_port);

        auto status = sender.outgoing_connection_status().need_pop().front().status;
        REQUIRE(status == broker::outgoing_connection_status::tag::established);

        //sender.advertise(topics[0]);
        sender.send(topics[0], msg);
        usleep(500 * 1000);

        REQUIRE(called.count(topics[0]));
        REQUIRE(called.at(topics[0]) == msg);
    }

    SECTION("Testing Receiver receive multiple queues success") {

        REQUIRE(called.size() == 0);

        rec.Listen(cb);
        usleep(500 * 1000);

        auto msg1 = broker::message{"msg1"};
        auto msg2 = broker::message{"msg2"};
        auto msg3 = broker::message{"msg3"};
        auto sender = broker::endpoint("sender");
        sender.peer(local_ip, local_port);

        auto status = sender.outgoing_connection_status().need_pop().front().status;
        REQUIRE(status == broker::outgoing_connection_status::tag::established);

        //sender.advertise(topics[0]);
        sender.send(topics[0], msg1);
        sender.send(topics[1], msg2);
        sender.send(topics[2], msg3);

        usleep(500 * 1000);

        REQUIRE(called.count(topics[0]));
        REQUIRE(called.count(topics[1]));
        REQUIRE(called.count(topics[2]));
        REQUIRE(called.at(topics[0]) == msg1);
        REQUIRE(called.at(topics[1]) == msg2);
        REQUIRE(called.at(topics[2]) == msg3);
    }
    // single queue -> receive okay
    // single queue -> receive fail
    // multi queue -> partial receive
    // multi queue -> full receive
    // multi queue -> full fail
}