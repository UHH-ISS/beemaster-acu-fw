//
// Sends data via broker to given destination + port.
//

#include <broker/broker.hh>
//#include <acu/logger.h>
#include "acu/sender.h"
#include "broker/endpoint.hh"

namespace acu {
    std::string const ENDPOINT_NAME = "ACU Sender";
    using namespace broker;

    bool Sender::Send(OutgoingAlert *alert) {
        //auto log = Logger::GetLogger("Sender");
        endpoint ep(ENDPOINT_NAME);

        // default retry interval is 5 seconds. Maybe we want to change that?
        peering success = ep.peer(this->destination, this->port);
        if ( !success | (ep.outgoing_connection_status().want_pop().front().status !=
             outgoing_connection_status::tag::established)) {
            //log->Warn("Sender: Error peering with", this->destination, this->port);
            return false;
        }

        // TODO: do we want to use the incidentName as topic?
        ep.send(alert->incidentName, alert->ToMessage());
        //log->Debug("Sender sent on topic", alert->incidentName);
        return true;
    }
}