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

#include "alert_mapper.h"
#include "aggregation.h"
#include "alert_mapper.h"
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
                : storage(storage), mapper(mapper),
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

        /// Check for new alerts. Should be called periodically.
        /// Allows the user to interact after incoming messages.
        void CheckForAlerts();

        /// Set connection data for the receiver.
        void SetReceiverInfo(std::string address, port_t port);

        /// Set connection data for the sender.
        void SetSenderInfo(std::string address, port_t port);

    protected:
        void OnReceive(const IncomingAlert*);

    private:
        // Ctor fields
        Storage *storage;
        AlertMapper *mapper;

        // Broker communication
        Receiver *receiver;
        Sender *sender;

        // detailed broker information
        std::string recv_address = "127.0.0.1";         // we ain't MongoDB. No need to expose per default.
        port_t recv_port = 9999;
        std::string send_address = "127.0.0.1";
        port_t send_port = 9998;

        // Internal communication
        std::queue<IncomingAlert*> *alertQueue;

        std::unordered_map<std::string, Aggregation*> *aggregations;
        std::unordered_map<std::string, Correlation*> *correlations;
    };
}


#endif //ACU_FW_ACU_H
