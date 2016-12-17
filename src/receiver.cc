/*
 * The ACU Receiver offers to fork and detach an asynchronous receiving broker endpoint.
 */

#include "acu/receiver.h"

#include <broker/message_queue.hh>
#include <sys/select.h>
#include <future>
#include <iostream>

namespace acu {

    // This could also be set via config file?
    std::string const ENDPOINT_NAME = "ACU Receiver";


    void DoListen(std::string address, port_t port,
                  std::vector<std::string> *topics,
                  std::function<void(const std::string, const broker::message&)> callback) {

        auto endpoint = new broker::endpoint(ENDPOINT_NAME, broker::AUTO_ADVERTISE);
        if (!endpoint->listen(port, address.c_str())) {
            //TODO: report error?
            return;
        }

        auto queues = new std::vector<broker::message_queue*>();
        for (auto &topic : *topics) {
            queues->push_back(new broker::message_queue(topic, *endpoint, broker::LOCAL_SCOPE));
        }

        fd_set fds;
        for (;;) {
            // Init fds
            FD_ZERO(&fds);
            for (auto q : *queues) {
                FD_SET(q->fd(), &fds);
            }
            // Block until at least one queue is ready to read
            auto result = select(FD_SETSIZE, &fds, nullptr, nullptr, nullptr);
            if (result == -1) {
                //TODO: Report error?
                return;
            }

            // Find readable queues
            for (auto &q : *queues) {
                if (FD_ISSET(q->fd(), &fds)) {
                    for (auto &msg : q->want_pop()) {
                        callback(q->get_topic_prefix(), msg);
                    }
                }
            }
        }
    }

    void Receiver::Listen(std::function<void(const std::string, const broker::message &)> callback) {
        // Fork an asynchronous receiver, return control flow / execution to caller:
        std::thread(DoListen, address, port, topics, callback).detach();
        return;
    }
}
