/* test_acu.cc
 * ACU Framework (test)
 *
 * Dataflow test for the ACU class.
 *
 * @author: 0ortmann
 */

#include "catch.hpp"

#include <acu/acu.h>
#include <broker/message_queue.hh>
#include <unistd.h>

class MockStorage : public acu::Storage {
    public:
        MockStorage(std::string db_name) : acu::Storage(db_name), persisted(false) {};
        bool persisted;
        void Persist(const acu::IncomingAlert *alert) {
            // supress a warning for unused alert
            alert->source_ip();
            persisted = true;
        }
};

class MockAggregation : public acu::Aggregation {
    public:
        MockAggregation(acu::Storage *storage, std::vector<acu::Threshold> *thresholds)
            : acu::Aggregation(storage, thresholds), invokes(0) {}
        uint16_t invokes;

        bool Invoke(const acu::IncomingAlert *alert) {
            // supress a warning for unused alert
            alert->source_ip();
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

TEST_CASE("Testing ACU roundtrip dataflow", "[acu]") {

    // This is kind of a "framework integration test"

    MockStorage *storage = new MockStorage("DB_NAME");
    acu::AlertMapper *mapper = new acu::AlertMapper();
    auto thr = acu::Threshold(1, "proto", "http");
    auto thresholds = new std::vector<acu::Threshold>();
    thresholds->push_back(thr);

    auto inc_topic = "TOPIC";
    auto alert_topic = "beemaster/acu/alert";
    auto topics = new std::vector<std::string>();
    topics->push_back(inc_topic);

    acu::Acu *acu = new acu::Acu(storage, mapper);
    MockAggregation *agg = new MockAggregation(storage, thresholds);
    auto mockAlertName = "META ALERT";
    auto mockAlertTime = std::chrono::system_clock::now();
    auto mockAlertTimeVal = std::chrono::duration_cast<std::chrono::duration<double>>(mockAlertTime.time_since_epoch());
    auto *mockAlert = new acu::OutgoingAlert(mockAlertName, mockAlertTime);
    MockCorrelation *corr = new MockCorrelation(storage, thresholds, mockAlert);

    acu->Register(topics, agg, corr);

    REQUIRE_FALSE(storage->persisted);
    REQUIRE(agg->invokes == 0);
    REQUIRE_FALSE(corr->correlated);

    // remote bro-broker "mock" via localhost
    broker::endpoint meta_alert_rec("Meta Alert Receiver",
                                    broker::AUTO_ROUTING | broker::AUTO_ADVERTISE);
    broker::message_queue meta_alert_queue(alert_topic, meta_alert_rec, broker::GLOBAL_SCOPE);

    acu->SetReceiverInfo("127.0.0.1", 9999);    // to make sure
    acu->SetSenderInfo("127.0.0.1", 9997);      // to prove a point ;)
    bool listening = meta_alert_rec.listen(9997, "127.0.0.1");
    usleep(100 * 1000);

    REQUIRE(listening);

    auto inc_alert_sender = broker::endpoint("incoming alert sender",
                                             broker::AUTO_ROUTING | broker::AUTO_PUBLISH);

    inc_alert_sender.listen(9999, "127.0.0.1");
    usleep(100 * 1000);

    std::cout << "ACU test peer" <<std::endl;
    acu->Run();

    // now we expect to have our ACU running, with a receiver that
    // peers against the inc_alert_sender on 127.0.0.1:9999
    // and a mocked meta-alert-receiver listening on 127.0.0.1:9998
    // to which out ACU sender will try to bind against

    // let everything take its time...!
    usleep(500 * 1000);

    // our ACU meta-alert sender should have connected to the mock meta-alert receiver (aka remote bro)
    REQUIRE(meta_alert_rec.incoming_connection_status().want_pop().front().status
            == broker::incoming_connection_status::tag::established);


    SECTION("Test successful roundtrip") {

        auto rec = broker::record({
                broker::record::field(broker::time_point{mockAlertTimeVal.count()}),
                broker::record::field("SOURCE_IP"),
                broker::record::field(1337),
                broker::record::field("DEST_IP"),
                broker::record::field(1338)
        });
        auto msg = broker::message{inc_topic, rec};

        // the fake incoming alert sender must have received a peer from our receiver
        REQUIRE(inc_alert_sender.incoming_connection_status().need_pop().front().status
                == broker::incoming_connection_status::tag::established);

        inc_alert_sender.send(inc_topic, msg);
        usleep(1000*300);

        // Process received alerts
        acu->CheckForAlerts();

        // framework should receive the sent message and auto-persist it
        REQUIRE(storage->persisted);

        // aggregation is registered, so it should be invoked exactly once
        REQUIRE(agg->invokes == 1);

        // The mocked aggregation triggers for correlation immediately, so our framework should invoke corr
        REQUIRE(corr->correlated);

        for (auto &msg : meta_alert_queue.need_pop()) {
            std::cout << "MetaAlert received: " << broker::to_string(msg) << std::endl;
            REQUIRE(msg.at(0) == "Beemaster::acu_meta_alert");
            //REQUIRE(msg.at(1) == mockAlertTimeVal.count()); // precision
            REQUIRE(msg.at(2) == mockAlertName);
        }
    }
}
