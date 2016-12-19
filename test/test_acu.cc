/*
 * Dataflow test for the ACU class.
 *
 * TODO: NOTE: This test works, because the hardcoded addresses inside the ACU are known
 * TODO: Note: Sender on 127.0.0.1:9998 and receiver on 127.0.0.1:9999 FIXME
 */

#include "catch.hpp"
#include <unistd.h>

#include <acu/acu.h>
#include <iostream>
#include <broker/broker.h>


TEST_CASE("Testing Acu class layout", "[acu]") {
    REQUIRE(std::is_copy_assignable<acu::Acu>());
    REQUIRE(std::is_copy_constructible<acu::Acu>());

    REQUIRE(std::is_move_assignable<acu::Acu>());
    REQUIRE(std::is_move_constructible<acu::Acu>());

    REQUIRE(std::is_standard_layout<acu::Acu>());
    REQUIRE_FALSE(std::is_pod<acu::Acu>());
}

class MockStorage : public acu::Storage {
public:
    MockStorage(std::string db_name) : acu::Storage(db_name), persisted(false) {};
    bool persisted;
    void Persist(acu::IncomingAlert *alert) {
        // supress a warning for unused alert
        alert->protocol();
        persisted = true;
    }
};

class MockAggregation : public acu::Aggregation {
    public:
        MockAggregation(acu::Storage *storage, std::vector<acu::Threshold> *thresholds)
            : acu::Aggregation(storage, thresholds), invokes(0) {}
        int16_t invokes;

        bool Invoke(acu::IncomingAlert *alert) {
            // supress a warning for unused alert
            alert->protocol();
            ++invokes;
            return invokes >= thresholds->at(0).count;
        }
};

class MockCorrelation : public acu::Correlation {
    public:
        MockCorrelation(acu::Storage *storage, std::vector<acu::Threshold> *thresholds)
            : acu::Correlation(storage, thresholds), correlated(false) {}
        bool correlated;

        void Invoke() {
            correlated = true;
        }
};

TEST_CASE("Testing ACU roundtrip dataflow", "[Acu]") {

    // This is kind of a "framework integration test"

    // Mock an alerttype + topic
    // verify receiver is started
    // send sth, assert receive
    // mock mapping
    // assert mapping for correct topic -> alertType
    // mock aggregation
    // assert invocation of aggregation
    // (on pos agg test: assert invocation of correlation)
    // (on pos corr test: assert sender sending)

    MockStorage *storage = new MockStorage("DB_NAME");
    auto thr = acu::Threshold(1, "proto", "http");
    auto thresholds = new std::vector<acu::Threshold>();
    thresholds->push_back(thr);

    auto topic = "TOPIC";
    auto topics = new std::vector<std::string>();
    topics->push_back(topic);


    acu::Acu *acu = new acu::Acu(storage);
    MockAggregation *agg = new MockAggregation(storage, thresholds);
    MockCorrelation *corr = new MockCorrelation(storage, thresholds);

    acu->Register(topics, agg, corr);

    REQUIRE_FALSE(storage->persisted);
    REQUIRE(agg->invokes == 0);
    REQUIRE_FALSE(corr->correlated);

    acu->Run();
    usleep(1000*300); // let our forked receiver grip

    // now we expect to have a receiver listening on 127.0.0.1:9999
    // TODO: that address is hardcoded. The test will break if this gets configurable

    SECTION("Test successful roundtrip") {

        auto msg = broker::message{
                "TIME",
                "INCIDENT_NAME",
                "PROTOCOL",
                "SOURCE_IP",
                1337,
                "DEST_IP",
                1338
        };
        auto sender = broker::endpoint("acu test sender");

        std::cout << "ACU test peer" <<std::endl;
        sender.peer("127.0.0.1", 9999);

        auto status = sender.outgoing_connection_status().need_pop().front().status;
        REQUIRE(status == broker::outgoing_connection_status::tag::established);

        sender.send(topic, msg);
        usleep(1000*300);

        REQUIRE(storage->persisted);

        REQUIRE(agg->invokes == 1);

        // TODO: so far nobody invokes the correlation LOL
        //REQUIRE(corr->correlated);
    }

}