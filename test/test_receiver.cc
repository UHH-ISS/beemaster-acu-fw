/* test_receiver.cc
 * ACU Framework (test)
 *
 * Tests default assignment behaviour.
 * Currently only positive tests are implemented. Negative tests are not
 * possible yet.
 *
 * @author: 0ortmann, 1wilkens
 */

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

    auto time_stamp = std::chrono::system_clock::now();
    auto val = std::chrono::duration_cast<std::chrono::duration<double>>(time_stamp.time_since_epoch());
    auto broker_stamp = broker::time_point{val.count()};

    SECTION("Testing Receiver receive single queue success") {
        auto local_port = (acu::port_t)9980;
        auto rec = acu::Receiver(local_ip, local_port, &topics, mapper);

        // Receiver was created with 3 known topics, so 3 queues are registered.
        // One queue will receive, that must result in triggering this tests callback exactly once
        REQUIRE(queue->size() == 0);

        auto sender = broker::endpoint("sender", broker::AUTO_ROUTING | broker::AUTO_PUBLISH | broker::AUTO_ADVERTISE);
        sender.listen(local_port, local_ip);
        usleep(100 * 1000);

        auto r = broker::record({
            broker::record::field(broker_stamp),
            broker::record::field("127.0.0.1"),
            broker::record::field((acu::port_t)8080),
            broker::record::field("192.168.0.1"),
            broker::record::field((acu::port_t)9090)
        });
        auto msg = broker::message{r};

        rec.Peer(queue);

        auto status = sender.incoming_connection_status().need_pop().front().status;
        REQUIRE(status == broker::incoming_connection_status::tag::established);
        usleep(100 * 1000);

        sender.send(topics[0], msg);
        usleep(100 * 1000);

        REQUIRE(queue->size() == 1);
        REQUIRE(*(queue->front()->topic) == topics[0]);
        REQUIRE_FALSE(*(queue->front()->topic) == topics[1]);
        REQUIRE(queue->front()->source_ip() == "127.0.0.1");
        REQUIRE(queue->front()->source_port() == 8080);
    }

    SECTION("Testing Receiver receive multiple queues success") {
        auto local_port = (acu::port_t)9981;
        auto rec = acu::Receiver(local_ip, local_port, &topics, mapper);

        // Receiver was created with 3 known topics, so 3 queues are registered.
        // All 3 queue swill receive, that must result in triggering this tests callback
        // exactly 3 times with three different messages.
        REQUIRE(queue->size() == 0);
        auto sender = broker::endpoint("sender", broker::AUTO_ROUTING | broker::AUTO_PUBLISH | broker::AUTO_ADVERTISE);
        sender.listen(local_port, local_ip);
        usleep(100 * 1000);

        auto r1 = broker::record({
            broker::record::field(broker_stamp),
            broker::record::field("127.0.0.1"),
            broker::record::field((acu::port_t)8080),
            broker::record::field("192.168.0.1"),
            broker::record::field((acu::port_t)9090)
        });
        auto r2 = broker::record({
            broker::record::field(broker_stamp),
            broker::record::field("127.0.0.1"),
            broker::record::field((acu::port_t)8080),
            broker::record::field("192.168.0.1"),
            broker::record::field((acu::port_t)9090)
        });
        auto r3 = broker::record({
            broker::record::field(broker_stamp),
            broker::record::field("127.0.0.1"),
            broker::record::field((acu::port_t)8080),
            broker::record::field("192.168.0.1"),
            broker::record::field((acu::port_t)9090)
        });
        auto msg1 = broker::message{r1};
        auto msg2 = broker::message{r2};
        auto msg3 = broker::message{r3};

        rec.Peer(queue);
        auto status = sender.incoming_connection_status().need_pop().front().status;
        REQUIRE(status == broker::incoming_connection_status::tag::established);

        usleep(100 * 1000);
        sender.send(topics[0], msg1);
        sender.send(topics[1], msg2);
        sender.send(topics[2], msg3);

        usleep(100 * 1000);

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
