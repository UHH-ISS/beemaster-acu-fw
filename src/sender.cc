//
// Sends data via broker to given destination + port.
//

#include <broker/broker.hh>
#include <iostream>
#include <unistd.h>
#include "acu/sender.h"
#include "broker/endpoint.hh"


namespace acu {
    // This could also be set via config file?
    std::string const ENDPOINT_NAME = "ACU Sender";

    using namespace broker;

    bool Sender::Send(OutgoingAlert *alert) {

        // TBD ob ein einmaliges peering im konstruktor sinnvoller wäre.
        // das hier ist erstmal die "safe" variante, mit der wir starten können.
        endpoint ep(ENDPOINT_NAME);

        // default retry interval is 5 seconds. Maybe we want to change that?
        ep.peer(destination, port);

        // ep.outgoing_connection_status().want_pop().front() returns a non-null,
        // default constructed outgoing_connection_status.
        // the default result of outgoing_connection_status.status is undefined.
        // since "status" is modeled as enum, undefined is not an option and the default enum is resolved.
        // The default enum is the first value enumerated in the enum.
        // That is "established".
        // So any outgoing_connection_status is always "established", when it is initialized and later on
        // it may be set to "disconnected". Why it does not start with "disconnected" is a riddle to me.
        usleep(100 * 1000);

        if (ep.outgoing_connection_status().want_pop().front().status
                != outgoing_connection_status::tag::established) {

            return false;
        }
        std::cout << "sender peer success with " + destination + ":";
        std::cout << port;
        std::cout << std::endl;
        // TODO: do we want to use the incidentName as topic?
        ep.send(alert->incidentName, alert->ToMessage());
        return true;
    }
}