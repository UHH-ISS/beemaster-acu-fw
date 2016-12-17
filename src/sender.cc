/*
 * Sends data via broker to given destination + port.
 */

#include <broker/broker.hh>
#include <iostream>
#include "acu/sender.h"

namespace acu {
    // This could also be set via config file?
    std::string const ENDPOINT_NAME = "ACU Sender";

    Sender::Sender(std::string destination, port_t port) :
            endpoint(new broker::endpoint(ENDPOINT_NAME)) {

        // default retry interval is 5 seconds. Maybe we want to change that?
        endpoint->peer(destination, port);
    }

    bool Sender::Send(OutgoingAlert *alert) {

        auto conn_stati = endpoint->outgoing_connection_status().want_pop();
        if (conn_stati.size() == 0
            || conn_stati.front().status != broker::outgoing_connection_status::tag::established) {

            return false;
        }
        // TODO: do we want to use the incidentName as topic?
        endpoint->send(alert->incidentName, alert->ToMessage());
        return true;
    }
}