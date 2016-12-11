//
// Created by florian on 12/1/16.
//

#include <broker/broker.hh>
//#include <acu/logger.h>
#include "acu/sender.h"
#include "broker/endpoint.hh"

namespace acu {
    std::string const ENDPOINT_NAME = "ACU Sender"; // TODO fix name
    using namespace broker;

    void Sender::Send(OutgoingAlert *alert) {
        //auto log = Logger::GetLogger("Sender");
        endpoint ep(ENDPOINT_NAME);

        // default retry interval is 5 seconds. Maybe we want to change that?
        peering success = ep.peer(this->destination, this->port);
        if ( !success | (ep.outgoing_connection_status().need_pop().front().status !=
             outgoing_connection_status::tag::established)) {
            //log->Warn("Sender: Error peering with", this->destination, this->port);
            return;
        }
        //log->Debug("Sender established connection to", this->destination, this->port);

        // TODO: do we want to use the incidentName as topic?
        ep.send(alert->incidentName, alert->ToMessage());
        //log->Debug("Sender sent on topic", alert->incidentName);
    }
}