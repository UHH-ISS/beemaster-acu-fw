//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_RECEIVER_H
#define ACU_FW_RECEIVER_H

#include "utils.h"

#include <broker/endpoint.hh>
#include <broker/message.hh>
#include <functional>
#include <string>
#include <vector>

namespace acu {

    class Receiver {
    public:
        Receiver(std::string address, port_t port, std::vector<std::string>* topics)
                : address(address), port(port), topics(topics) {};

        // TODO: The broker message could/should be a reference to transfer ownership to the acu
        void Listen(std::function<void(const std::string, const broker::message&)>);

    private:
        std::string address;
        port_t port;
        std::vector<std::string> *topics;
    };
}


#endif //ACU_FW_RECEIVER_H
