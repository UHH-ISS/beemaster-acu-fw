/* alert_mapper.h
 * ACU Framework
 *
 * The alert mapper is to be implemented by the _user_ and maps incoming raw
 * broker messages to the corresponding incoming alerts.
 *
 * @author: 0ortmann, 1wilkens
 */

#ifndef ACU_FW_ALERT_MAPPER_H
#define ACU_FW_ALERT_MAPPER_H

#include "incoming_alert.h"

namespace acu {

    class AlertMapper {
    public:
        /// Factory method to convert a raw Broker message into the
        /// corresponding IncomingAlert-Subclass.
        ///
        /// @param topic    The topic string to identify the message.
        /// @param message  The message to convert.
        /// @return         The correctly mapped message.
        virtual IncomingAlert* GetAlert(const std::string, const broker::message&) const;
    };
}
#endif //ACU_FW_ALERT_MAPPER_H
