/* acu.h
 * ACU Framework
 *
 * The ACU is the entry-point to the whole framework. It manages all necessary
 * services and delegates messages between Receiver, Algorithms and Sender.
 *
 * @author: 1wilkens, 0ortmann, 1jost
 */

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
        /// @param storage  The concrete storage implementation to use for
        ///                 storing every incoming alert.
        /// @param mapper   The concrete alert mapper to be used to convert raw
        ///                 Broker messages to IncomingAlerts.
        Acu(Storage *storage, AlertMapper *mapper)
                : mapper(mapper), storage(storage),
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
        AlertMapper *mapper;
        Storage *storage;

        // Broker communication
        Receiver *receiver;
        Sender *sender;

        std::unordered_map<std::string, Aggregation*> *aggregations;
        std::unordered_map<std::string, Correlation*> *correlations;
    };
}


#endif //ACU_FW_ACU_H
