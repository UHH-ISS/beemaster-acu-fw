//
// Created by florian on 12/1/16.
//

#include "acu/receiver.h"
#include "acu/utils.h"
#include "acu/logger.h"

using namespace acu;

Receiver::Receiver(std::string address, port_t port, std::string topics[]) {
    this->endpoints = new endpoints*[sizeof(topics)];
    // for topic in topics:
    //   this->endpoints[
}
