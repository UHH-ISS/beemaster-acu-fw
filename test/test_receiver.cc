//
// Created by florian on 12/16/16.
//

#include "catch.hpp"

#include <acu/receiver.h>

#include <algorithm>
#include <iostream>
#include <unistd.h>

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
    auto local_ip = "127.0.0.1";
    auto mapper = new acu::AlertMapper();
    auto queue = new std::queue<acu::IncomingAlert*>();

    SECTION("Testing Receiver receive single queue success") {
        auto local_port = (acu::port_t)9980;
        auto rec = acu::Receiver(local_ip, local_port, &topics, mapper);

        // Receiver was created with 3 known topics, so 3 queues are registered.
        // One queue will receive, that must result in triggering this tests callback exactly once
        REQUIRE(queue->size() == 0);

        rec.Listen(queue);
        usleep(500 * 1000);

        auto msg = broker::message{"testmessage", "incident", "proto", "4", "5", "6", "7"};
        auto sender = broker::endpoint("sender");
        sender.peer(local_ip, local_port);

        auto status = sender.outgoing_connection_status().need_pop().front().status;
        REQUIRE(status == broker::outgoing_connection_status::tag::established);

        sender.send(topics[0], msg);
        usleep(500 * 1000);

        REQUIRE(queue->size() == 1);
        REQUIRE(*(queue->front()->topic) == topics[0]);
        REQUIRE_FALSE(*(queue->front()->topic) == topics[1]);
        REQUIRE(queue->front()->incident_type() == "incident");
        REQUIRE(queue->front()->protocol() == "proto");
    }

    SECTION("Testing Receiver receive multiple queues success") {
        auto local_port = (acu::port_t)9981;
        auto rec = acu::Receiver(local_ip, local_port, &topics, mapper);

        // Receiver was created with 3 known topics, so 3 queues are registered.
        // All 3 queue swill receive, that must result in triggering this tests callback
        // exactly 3 times with three different messages.
        REQUIRE(queue->size() == 0);

        rec.Listen(queue);
        usleep(500 * 1000);

        auto msg1 = broker::message{"msg1", "2", "3", "4", "5", "6", "7"};
        auto msg2 = broker::message{"msg2", "2", "3", "4", "5", "6", "7"};
        auto msg3 = broker::message{"msg3", "2", "3", "4", "5", "6", "7"};
        auto sender = broker::endpoint("sender");
        sender.peer(local_ip, local_port);

        auto status = sender.outgoing_connection_status().need_pop().front().status;
        REQUIRE(status == broker::outgoing_connection_status::tag::established);

        sender.send(topics[0], msg1);
        sender.send(topics[1], msg2);
        sender.send(topics[2], msg3);

        usleep(500 * 1000);

        REQUIRE(queue->size() == 3);
        for (int i = 0; i < 3; ++i) {
            auto alert = queue->front();
            queue->pop();
            REQUIRE(std::find(topics.begin(), topics.end(), *alert->topic) != topics.end());
        }
    }
    // single queue -> receive fail TODO
    // multi queue -> full fail TODO
}