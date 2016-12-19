#include <iostream>
#include "acu/acu.h"

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

        //TODO: mapper parameter?
        mapper = new AlertMapper();

        // Async fork a listening thread
        receiver->Listen(std::bind(&Acu::OnReceive, this, std::placeholders::_1, std::placeholders::_2));

        // This keeps another thred running, we do not need to loop here
    }

    void Acu::OnReceive(const std::string topic, const broker::message &message) {
        std::cout << "Acu OnReceive for topic " + topic << std::endl;
        if (!aggregations->count(topic)) {
            // TODO Log error
            return;
        }

        IncomingAlert *alert = mapper->GetAlert(topic, message);

        storage->Persist(alert);
        if (aggregations->at(topic)->Invoke(alert)) {
            correlations->at(topic)->Invoke();
        }
    }
}
