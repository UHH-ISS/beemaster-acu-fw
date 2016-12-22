/*
 * Dataflow test for the ACU class.
 *
 * TODO: NOTE: This test works, because the hardcoded addresses inside the ACU are known
 * TODO: Note: Sender on 127.0.0.1:9998 and receiver on 127.0.0.1:9999 FIXME
 */

#include "catch.hpp"

#include <acu/acu.h>
#include <broker/message_queue.hh>
#include <iostream>
#include <unistd.h>

class MockStorage : public acu::Storage {
    public:
        MockStorage(std::string db_name) : acu::Storage(db_name), persisted(false) {};
        bool persisted;
        void Persist(const acu::IncomingAlert *alert) {
            // supress a warning for unused alert
            alert->protocol();
            persisted = true;
        }
};

class MockOutgoingAlert : public acu::OutgoingAlert {
    public:
        MockOutgoingAlert(std::string name, std::chrono::time_point<std::chrono::system_clock> timestamp)
                : acu::OutgoingAlert(name, timestamp), toMessageCalled(false) {};
        bool toMessageCalled;
        const broker::message ToMessage() {
            toMessageCalled = true;
            return acu::OutgoingAlert::ToMessage();
        }
};


class MockAggregation : public acu::Aggregation {
    public:
        MockAggregation(acu::Storage *storage, std::vector<acu::Threshold> *thresholds)
            : acu::Aggregation(storage, thresholds), invokes(0) {}
        int16_t invokes;

        bool Invoke(const acu::IncomingAlert *alert) {
            // supress a warning for unused alert
            alert->protocol();
            ++invokes;
            return invokes >= thresholds->at(0).count;
        }
};

class MockCorrelation : public acu::Correlation {
    public:
        MockCorrelation(acu::Storage *storage, std::vector<acu::Threshold> *thresholds, acu::OutgoingAlert *alert)
            : acu::Correlation(storage, thresholds), correlated(false), dummyAlert(alert) {}
        bool correlated;
        acu::OutgoingAlert *dummyAlert;

        acu::OutgoingAlert* Invoke() {
            correlated = true;
            return dummyAlert;
        }
};


TEST_CASE("Testing Acu class layout", "[acu]") {
    REQUIRE(std::is_copy_assignable<acu::Acu>());
    REQUIRE(std::is_copy_constructible<acu::Acu>());

    REQUIRE(std::is_move_assignable<acu::Acu>());
    REQUIRE(std::is_move_constructible<acu::Acu>());

    REQUIRE(std::is_standard_layout<acu::Acu>());
    REQUIRE_FALSE(std::is_pod<acu::Acu>());
}

TEST_CASE("Testing ACU roundtrip dataflow", "[Acu]") {

    // This is kind of a "framework integration test"

    MockStorage *storage = new MockStorage("DB_NAME");
    acu::AlertMapper *mapper = new acu::AlertMapper();
    auto thr = acu::Threshold(1, "proto", "http");
    auto thresholds = new std::vector<acu::Threshold>();
    thresholds->push_back(thr);

    auto topic = "TOPIC";
    auto topics = new std::vector<std::string>();
    topics->push_back(topic);

    acu::Acu *acu = new acu::Acu(storage, mapper);
    MockAggregation *agg = new MockAggregation(storage, thresholds);
    auto mockAlertName = "META ALERT";
    auto mockAlertTime = std::chrono::system_clock::now();
    MockOutgoingAlert *mockAlert = new MockOutgoingAlert(mockAlertName, mockAlertTime);
    MockCorrelation *corr = new MockCorrelation(storage, thresholds, mockAlert);

    acu->Register(topics, agg, corr);

    REQUIRE_FALSE(storage->persisted);
    REQUIRE(agg->invokes == 0);
    REQUIRE_FALSE(corr->correlated);
    REQUIRE_FALSE(mockAlert->toMessageCalled);

    // remote bro-broker "mock" via localhost
    broker::endpoint meta_alert_rec("Meta Alert Receiver");
    broker::message_queue meta_alert_queue(topic, meta_alert_rec);

    bool listening = meta_alert_rec.listen(9998, "127.0.0.1");
    REQUIRE(listening);

    acu->Run();
    // now we expect to have our ACU receiver listening on 127.0.0.1:9999
    // and a mocked meta-alert receiver listening on 127.0.0.1:9998
    // our ACU sender will try to bind against the meta-alert receiver
    // TODO: those addresses are hardcoded. The test will break if they get configurable

    // let everything take its time...!
    usleep(500 * 1000);

    // our ACU meta-alert sender should have connected to the mock meta-alert receiver (aka remote bro)
    REQUIRE(meta_alert_rec.incoming_connection_status().want_pop().front().status
            == broker::incoming_connection_status::tag::established);


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
        auto inc_alert_sender = broker::endpoint("incoming alert sender");

        std::cout << "ACU test peer" <<std::endl;
        inc_alert_sender.peer("127.0.0.1", 9999);

        // the fake incoming alert sender must be able to bind against our ACU receiver
        REQUIRE(inc_alert_sender.outgoing_connection_status().need_pop().front().status
                == broker::outgoing_connection_status::tag::established);

        inc_alert_sender.send(topic, msg);
        usleep(1000*300);

        // Process received alerts
        acu->CheckForAlerts();

        // framework should receive the sent message and auto-persist it
        REQUIRE(storage->persisted);

        // aggregation is registered, so it should be invoked exactly once
        REQUIRE(agg->invokes == 1);

        // The mocked aggregation triggers for correlation immediately, so our framework should invoke corr
        REQUIRE(corr->correlated);

        // The mocked correlation always returns a static meta alert, so our framework should send that
        REQUIRE(mockAlert->toMessageCalled);

        for (auto &msg : meta_alert_queue.want_pop()) {
            REQUIRE(msg.at(0) == mockAlertTime.time_since_epoch().count());
            REQUIRE(msg.at(1) == mockAlertName);
        }
    }
}