#include "acu/acu.h"

namespace acu {

    void Acu::Register(std::vector<std::string> *topics, Aggregation *aggregation, Correlation *correlation) {
        // asserts
        assert(topics != nullptr);
        assert(correlation != nullptr);

        // iterate of topics to add maps
        for (auto topic : topics) {
            if (aggregations.count(topic)) {
                aggregations.emplace(topic, aggregation);
            }
            correlations.emplace(topic, correlation);
        }
    }
 }
