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

        receiver = new Receiver("127.0.0.1", 9999, topics);
        sender = new Sender("127.0.0.1", 9998);



        // TODO: RUN
        receiver->Listen(nullptr);
    }
}
