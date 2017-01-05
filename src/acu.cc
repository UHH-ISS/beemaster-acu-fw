/* acu.cc
 * ACU Framework
 * 
 * <include/acu/acu.h>
 */

#include "acu/acu.h"

#include <iostream>

namespace acu {

    void Acu::Register(std::vector<std::string> *topics, Aggregation *aggregation, Correlation *correlation) {
        // asserts
        assert(topics != nullptr);
        assert(correlation != nullptr);

        // iterate of topics to add maps
        for (auto &topic : *topics) {
            if (aggregation != nullptr) {
                aggregations->emplace(topic, aggregation);
            }
            correlations->emplace(topic, correlation);
        }
    }

    void Acu::Run() {
        auto topics = new std::vector<std::string>(correlations->size());
        for (auto &pair : *correlations) {
            topics->push_back(pair.first);
        }

        // TODO: config?
        receiver = new Receiver("127.0.0.1", 9999, topics, mapper);
        sender = new Sender("127.0.0.1", 9998);

        // Async fork a listening thread
        receiver->Listen(alertQueue);
    }

    void Acu::CheckForAlerts() {
        while (!alertQueue->empty()) {
            auto alert = alertQueue->front();
            OnReceive(alert);
            alertQueue->pop();
        }
    }

    void Acu::OnReceive(const IncomingAlert *alert) {
        auto topic = alert->topic;

        storage->Persist(alert);

        bool aggregated = !aggregations->count(*topic) || aggregations->at(*topic)->Invoke(alert);
        if (aggregated) {
            OutgoingAlert *outgoing = correlations->at(*topic)->Invoke();
            if (outgoing != nullptr) {
                sender->Send(outgoing);
            }
        }
    }
}
