/* outgoing_alert.cc
 * ACU Framework
 *
 * <include/acu/outgoing_alert.h>
 */

#include "acu/outgoing_alert.h"

namespace acu {

    const std::string OutgoingAlert::EventName() const {
        return "Beemaster::acu_meta_alert";
    }

    const broker::message OutgoingAlert::ToMessage() const {
        auto truncated = std::chrono::duration_cast<std::chrono::duration<double>>(timestamp.time_since_epoch());
        auto broker_stamp = broker::time_point{truncated.count()};
        return broker::message{EventName(), broker_stamp, incidentName};
    }
}
