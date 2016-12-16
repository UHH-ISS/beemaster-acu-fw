//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_RECEIVER_H
#define ACU_FW_RECEIVER_H

#include "utils.h"

#include <cstdint>
#include <string>
#include <vector>

namespace acu {

    class Receiver {
    public:
        Receiver(std::string address, port_t port, std::vector<std::string> *topics);

        void Listen();
    };
}


#endif //ACU_FW_RECEIVER_H
