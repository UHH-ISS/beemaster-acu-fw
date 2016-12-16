#ifndef ACU_FW_ACU_H
#define ACU_FW_ACU_H

#include "aggregation.h"
#include "correlation.h"
#include "receiver.h"
#include "sender.h"

#include <unordered_map>

namespace acu {

    class Acu {
    public:
        Acu() : aggregations(new std::unordered_map<std::string, Aggregation*>()),
                correlations(new std::unordered_map<std::string, Correlation*>()) {};

        /// Register aggregations and correlations on the given topics.
        ///
        /// **aggregation** may be NULL.
        ///
        /// @param topics       The topics to register to.
        /// @param aggregation  The Aggregation to register on those topics.
        /// @param correlation  The Correlation to register on those topics.
        void Register(std::vector<std::string> *topics, Aggregation *aggregation, Correlation *correlation);

        void Run();

    private:
        Receiver *receiver;
        Sender *sender;

        std::unordered_map<std::string, Aggregation*> *aggregations;
        std::unordered_map<std::string, Correlation*> *correlations;
    };
}


#endif //ACU_FW_ACU_H
