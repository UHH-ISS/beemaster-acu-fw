/* incoming_alert.h
 * ACU Framework
 *
 * The abstract class IncomingAlert is a proxy to the incoming broker message.
 * As such, the realization lies upon the implementor, as it is to match the
 * precise messages.
 *
 * @author: 1wilkens
 */

#ifndef ACU_FW_INCOMING_ALERT_H
#define ACU_FW_INCOMING_ALERT_H

#include "utils.h"

#include <broker/message.hh>
#include <chrono>
#include <iostream>
#include <string>

namespace acu {
    using namespace std::chrono;

    class IncomingAlert {
    public:
        /// Instantiate a new IncomingAlert.
        ///
        /// This type allows for easy, controlled access to the underlying
        /// message while trying to reduce complexity and memory.
        ///
        /// \param topic        The topic, this message was send on.
        /// \param message      The actual message.
        explicit IncomingAlert(const std::string*, const broker::message&);

        /// Topic the IncomingAlert was received under
        const std::string *topic;

        /// Timestamp indicating when the alert occurred
        time_point<system_clock> timestamp() const;

        /// Source IP of the connection that triggered this alert
        const std::string& source_ip() const;
        /// Source port of the connection that triggered this alert
        const port_t& source_port() const;

        /// Destination IP of the connection that triggered this alert
        const std::string& destination_ip() const;

        /// Destination port of the connection that triggered this alert
        const port_t& destination_port() const;

        //== operators ==

        /// Checks upon equality
        virtual bool operator==(const IncomingAlert&) const;
        /// Checks upon inequality
        virtual bool operator!=(const IncomingAlert&) const;

        /// Allows to print the alert into an ostream.
        friend std::ostream& operator<< (std::ostream &stream, const IncomingAlert &alert);

    protected:
        /// The underlying message
        const std::vector<broker::data> message;
    };
} // namespace acu

#endif //ACU_FW_INCOMING_ALERT_H
