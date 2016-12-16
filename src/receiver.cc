//
// Created by florian on 12/1/16.
//

#include "acu/receiver.h"

#include <broker/message_queue.hh>
#include <sys/select.h>
#include <future>
#include <iostream>

namespace acu {

    void DoListen(std::string address, port_t port,
                  std::vector<std::string> *topics,
                  std::function<void(const std::string, const broker::message&)> callback) {

        auto endpoint = new broker::endpoint("recv_endpoint", broker::AUTO_ADVERTISE);
        if (!endpoint->listen(port, address.c_str())) {
            //TODO: report error?
            return;
        }

        auto queues = new std::vector<broker::message_queue*>();
        for (auto &topic : *topics) {
            queues->push_back(new broker::message_queue(topic, *endpoint, broker::LOCAL_SCOPE));
        }

        for (;;) {
            for (auto &q : *queues) {
                for (auto &msg : q->want_pop()) {
                    std::cout << "halleluJAH" << std::endl;
                    callback(q->get_topic_prefix(), msg);
                    return;
                }
            }
        }

        /*fd_set fds;
        for (;;) {
            // Init fds
            FD_ZERO(&fds);
            for (auto q : *queues) {
                FD_SET(q->fd(), &fds);
            }

            std::cout << "calling select" << std::endl;
            // Block until at least one queue is ready to read
            auto result = select((int)(queues->size() + 1), &fds, nullptr, nullptr, nullptr);
            if (result == -1) {
                //TODO: Report error?
                return;
            }
            std::cout << "called select" << std::endl;

            // Find readable queues
            for (auto &q : *queues) {
                if (FD_ISSET(q->fd(), &fds)) {
                    for (auto &msg : q->want_pop()) {
                        std::cout << "calling callback" << std::endl;
                        callback(q->get_topic_prefix(), msg);
                    }
                }
            }
        }*/
    }

    void Receiver::Listen(std::function<void(const std::string, const broker::message &)> callback) {
        std::cout << "pre" << std::endl;
        std::thread(DoListen, address, port, topics, callback).detach();
        std::cout << "post" << std::endl;
        return;
    }
}
