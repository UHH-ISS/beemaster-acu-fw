/* sender.cc
 * ACU Framework
 *
 * Sends data via broker to given destination + port.
 *
 * <include/acu/sender.h>
 */

#include "acu/sender.h"

#include <broker/broker.hh>
#include <iostream>

namespace acu {
    // This could also be set via config file?
    const std::string Sender::ACU_OUTGOING_ALERT_TOPIC = "beemaster/acu/alert";
    const std::string Sender::ENDPOINT_NAME = "acu_sender";


    Sender::Sender(std::string destination, port_t port)
            : endpoint(new broker::endpoint(ENDPOINT_NAME, broker::AUTO_ROUTING | broker::AUTO_PUBLISH)) {

        // default retry interval is 5 seconds. Maybe we want to change that?
        endpoint->peer(destination, port);
    }

    bool Sender::Send(OutgoingAlert *alert) const {
        endpoint->send(ACU_OUTGOING_ALERT_TOPIC, alert->ToMessage());
        return true;
    }
}
