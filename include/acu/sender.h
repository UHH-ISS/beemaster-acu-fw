/* sender.h
 * ACU Framework
 *
 * The sender sends messages to the given site.
 *
 * @author: 0ortmann
 */

#ifndef ACU_FW_SENDER_H
#define ACU_FW_SENDER_H

#include "outgoing_alert.h"
#include "utils.h"

#include <broker/endpoint.hh>
#include <string>

namespace acu {

    class Sender {
    public:
        const static std::string ACU_OUTGOING_ALERT_TOPIC;
        const static std::string ENDPOINT_NAME;

        Sender(std::string destination, port_t port);
        bool Send(OutgoingAlert *alert) const;

    private:
        broker::endpoint *endpoint;
    };
}


#endif //ACU_FW_SENDER_H
