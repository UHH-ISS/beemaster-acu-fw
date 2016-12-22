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
