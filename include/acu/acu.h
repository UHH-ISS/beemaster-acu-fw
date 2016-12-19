#ifndef ACU_FW_ACU_H
#define ACU_FW_ACU_H

#include "aggregation.h"
#include "correlation.h"
#include "receiver.h"
#include "sender.h"
#include "alert_mapper.h"

#include <unordered_map>

namespace acu {

    class Acu {
    public:
        /// @param storage  The concrete storage implementation to use for storing every incoming alert.
        Acu(Storage *storage)
                : storage(storage),
                  aggregations(new std::unordered_map<std::string, Aggregation*>()),
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

    protected:
        void OnReceive(const std::string topic, const broker::message &message);

    private:
        Receiver *receiver;
        Sender *sender;
        AlertMapper *mapper;
        Storage *storage;

        std::unordered_map<std::string, Aggregation*> *aggregations;
        std::unordered_map<std::string, Correlation*> *correlations;
    };
}


#endif //ACU_FW_ACU_H
