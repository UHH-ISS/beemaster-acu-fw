//
// OutgoingAlert is the basic abstract class for "meta-alerts" sent back from the ACU framework to the Bro-Master.
//

#include "acu/outgoing_alert.h"

namespace acu {

    const broker::message OutgoingAlert::AsMessage() {
        return broker::message{this->timestamp.time_since_epoch().count(), this->incidentName};
    }
}
