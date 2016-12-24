/* outgoing_alert.cc
 * ACU Framework
 * 
 * <include/acu/outgoing_alert.h>
 */

#include "acu/outgoing_alert.h"

namespace acu {

    const broker::message OutgoingAlert::ToMessage() {
        return broker::message{timestamp.time_since_epoch().count(), incidentName};
    }
}
