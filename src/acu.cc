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
        receiver = new Receiver("127.0.0.1", 9999, topics);
        sender = new Sender("127.0.0.1", 9998);

        // Async fork a listening thread
        receiver->Listen(std::bind(&Acu::OnReceive, this, std::placeholders::_1, std::placeholders::_2));

        // This keeps another thread running, we do not need to loop here
    }

    void Acu::OnReceive(const std::string topic, const broker::message &message) {
        std::cout << "Acu OnReceive for topic " + topic << std::endl;

        IncomingAlert *alert = mapper->GetAlert(topic, message);

        if (alert == nullptr) {
            //TODO: "Log no mapping"
            return;
        }

        storage->Persist(alert);

        bool aggregated = !aggregations->count(topic) || aggregations->at(topic)->Invoke(alert);

        if (aggregated) {
            OutgoingAlert *outgoing = correlations->at(topic)->Invoke();
            if (outgoing != nullptr) {
                sender->Send(outgoing);
            }
        }
    }
}
