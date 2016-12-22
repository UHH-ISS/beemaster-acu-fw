#ifndef ACU_FW_ACU_H
#define ACU_FW_ACU_H

#include "alert_mapper.h"
#include "aggregation.h"
#include "correlation.h"
#include "receiver.h"
#include "sender.h"

#include <unordered_map>

namespace acu {

    class Acu {
    public:
        /// @param storage  The concrete storage implementation to use for
        ///                 storing every incoming alert.
        /// @param mapper   The concrete alert mapper to be used to convert raw
        ///                 Broker messages to IncomingAlerts.
        Acu(Storage *storage, AlertMapper *mapper)
                : mapper(mapper), storage(storage),
                  alertQueue(new std::queue<IncomingAlert*>()),
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

        /// Start the ACU main loop
        void Run();

        void CheckForAlerts();

    protected:
        void OnReceive(const IncomingAlert*);

    private:
        AlertMapper *mapper;
        Storage *storage;

        // Broker communication
        Receiver *receiver;
        Sender *sender;

        std::queue<IncomingAlert*> *alertQueue;

        std::unordered_map<std::string, Aggregation*> *aggregations;
        std::unordered_map<std::string, Correlation*> *correlations;
    };
}


#endif //ACU_FW_ACU_H
