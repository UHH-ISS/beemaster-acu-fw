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
        return broker::message{EventName(), timestamp.time_since_epoch().count(), incidentName};
    }
}
