//
// Created by florian on 12/16/16.
//

#include "catch.hpp"

#include <acu/receiver.h>

#include <unistd.h>
#include <iostream>

TEST_CASE("Testing Receiver class layout", "[receiver]") {

}

TEST_CASE("Testing Receiver receive", "[receiver]") {
    // Setup
    auto topics = std::vector<std::string>{
        "topic1",
        "topic2"
    };
    auto rec = acu::Receiver("127.0.0.1", 9980, &topics);

    auto called = std::map<std::string, broker::message>();
    auto cb = [&](const std::string topic, const broker::message &msg) {
        std::cout << "called callback in test" << std::endl;
        called.emplace(topic, msg);
    };

    SECTION("Testing Receiver receive single queue success") {
        rec.Listen(cb);

        sleep(1);

        auto msg = broker::message{"testmessage"};
        auto sender = broker::endpoint("sender");
        auto peering = sender.peer("127.0.0.1", 9980);
        std::cout << "after peering" << std::endl;

        auto status = sender.outgoing_connection_status().need_pop().front().status;
        REQUIRE(status == broker::outgoing_connection_status::tag::established);

        sleep(1);
        sender.advertise(topics[0]);
        sender.send(topics[0], msg);
        sleep(1);

        REQUIRE(called.count(topics[0]));
        //REQUIRE(called.at(topics[0]) == msg);
    }
    // single queue -> receive okay
    // single queue -> receive fail
    // multi queue -> partial receive
    // multi queue -> full receive
    // multi queue -> full fail
}