/* receiver.cc
 * ACU Framework
 *
 * The ACU Receiver offers to fork and detach an asynchronously receiving broker endpoint.
 *
 * <include/acu/receiver.h>
 */

#include "acu/receiver.h"

#include <broker/message_queue.hh>
#include <future>
#include <iostream>
#include <sys/select.h>

namespace acu {

    void DoPeer(std::string address, port_t port, std::vector<std::string> *topics,
                  AlertMapper *mapper, std::queue<IncomingAlert*> *alertQueue) {
        auto endpoint = new broker::endpoint(ENDPOINT_NAME,
                                             broker::AUTO_ROUTING | broker::AUTO_PUBLISH | broker::AUTO_ADVERTISE);
        endpoint->peer(address.c_str(), port);

        auto queues = new std::vector<broker::message_queue*>();
        for (auto &topic : *topics) {
            queues->push_back(new broker::message_queue(topic, *endpoint, broker::GLOBAL_SCOPE));
        }

        fd_set fds;
        for (;;) {
            // Init fds
            FD_ZERO(&fds);
            for (auto q : *queues) {
                FD_SET(q->fd(), &fds);
            }
            // Block until at least one alertQueue is ready to read
            auto result = select(FD_SETSIZE, &fds, nullptr, nullptr, nullptr);
            if (result == -1) {
                //TODO: Report error?
                return;
            }

            // Find readable queues
            for (auto &q : *queues) {
                if (FD_ISSET(q->fd(), &fds)) {
                    auto topic = q->get_topic_prefix();
                    if (q->get_topic_prefix() != "") {
                        for (auto &msg : q->want_pop()) {
                            try {
                                auto alert = mapper->GetAlert(new std::string(topic), msg);
                                if (alert != nullptr) {
                                    alertQueue->emplace(alert);
                                }
                            }
                            catch (std::invalid_argument*) {
                                std::cout << "Failed to map alert of topic: " << topic << std::endl;
                                std::cout << "Discarding message.." << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }

    void Receiver::Peer(std::queue<IncomingAlert*> *queue) {
        // Fork an asynchronous receiver, return control flow / execution to caller:
        std::thread(DoPeer, address, port, topics, mapper, queue).detach();
        return;
    }
}
