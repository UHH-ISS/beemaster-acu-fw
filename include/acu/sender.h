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
        /// Constant topic name for outgoing alerts.
        const static std::string ACU_OUTGOING_ALERT_TOPIC;
        /// Constant name of the senders endpoint.
        const static std::string ENDPOINT_NAME;

        /// Instantiate a Sender
        ///
        /// \param destination      The destination address to send to
        /// \param port             The destination port to send to
        Sender(std::string destination, port_t port);

        /// Send the given alert.
        ///
        /// \param alert    The OutgoingAlert to send
        /// \return         True if successful, false otherwise
        bool Send(OutgoingAlert *alert) const;

    private:
        broker::endpoint *endpoint;
    };
}


#endif //ACU_FW_SENDER_H
