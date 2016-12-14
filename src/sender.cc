//
// Sends data via broker to given destination + port.
//

#include <broker/broker.hh>
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
        peering success = ep.peer(this->destination, this->port);
        if (!success || ep.outgoing_connection_status().want_pop().front().status
                        != outgoing_connection_status::tag::established) {
            return false;
        }

        // TODO: do we want to use the incidentName as topic?
        ep.send(alert->incidentName, alert->ToMessage());
        return true;
    }
}