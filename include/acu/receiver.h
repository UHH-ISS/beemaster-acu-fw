//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_RECEIVER_H
#define ACU_FW_RECEIVER_H

#include <cstdint>
#include <string>

#include <broker/endpoint.hh>

#include "acu/utils.h"

namespace acu {

    class Receiver {
    public:
        /// Initialise a new Receiver.
        ///
        /// \param address  The address to listen on.
        /// \param port     The port to listen on.
        /// \param topics   The topics to subscribe to.
        Receiver(std::string address, port_t port, std::string topics[]);

        void Listen();
    private:
        broker::endpoint* endpoints[];
    };

    const static std::string ENDPOINT_PREFIX = "acu_receiver";
}


#endif //ACU_FW_RECEIVER_H
