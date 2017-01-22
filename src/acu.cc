/* acu.cc
 * ACU Framework
 * 
 * <include/acu/acu.h>
 */

#include "acu/acu.h"

#include <iostream>

namespace acu {
    void Acu::SetReceiverInfo(std::string address, port_t port) {
        recv_address = address;
        recv_port = port;
    }

    void Acu::SetSenderInfo(std::string address, port_t port) {
        send_address = address;
        send_port = port;
    }

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

        receiver = new Receiver(recv_address, recv_port, topics, mapper);
        sender = new Sender(send_address, send_port);

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
