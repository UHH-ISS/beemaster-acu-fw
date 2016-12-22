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

#include <string>
#include <broker/endpoint.hh>

namespace acu {

    class Sender {
    public:
        Sender(std::string destination, port_t port);
        bool Send(OutgoingAlert *alert);

    private:
        broker::endpoint *endpoint;
    };
}


#endif //ACU_FW_SENDER_H
