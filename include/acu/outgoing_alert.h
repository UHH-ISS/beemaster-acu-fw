/* outgoing_alert.h
 * ACU Framework
 *
 * The abstract class OutgoingAlert is used as a result from aggregation
 * algorithms and are able to produce broker messages.
 *
 * @author: 1wilkens, 0ortmann
 */

#ifndef ACU_FW_OUTGOING_ALERT_H
#define ACU_FW_OUTGOING_ALERT_H

#include <broker/message.hh>
#include <chrono>
#include <string>

namespace acu {
    using namespace std::chrono;

    class OutgoingAlert {
    public:
        /// Instantiate an OutgoingAlert.
        ///
        /// \param name         Name of the alert-event
        /// \param timestamp    The point in time, this event occured
        OutgoingAlert(std::string name, time_point<system_clock> timestamp)
                : incidentName(name), timestamp(timestamp) {};

        /// The name of this incident/alert.
        std::string incidentName;
        /// The time_point to this incident/alert.
        time_point<system_clock> timestamp;

        /// \return     The name of this event.
        virtual const std::string EventName() const;
        /// \return     The broker-message of this alert.
        virtual const broker::message ToMessage() const;
    };
}


#endif //ACU_FW_OUTGOING_ALERT_H
