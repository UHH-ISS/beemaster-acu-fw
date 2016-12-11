//
// OutgoingAlert is the basic abstract class for "meta-alerts" sent back from the ACU framework to the Bro-Master.
//

#include "acu/outgoing_alert.h"

namespace acu {

    const broker::message OutgoingAlert::AsMessage() {
        //auto name = broker_string_create(this->name.c_str());
        //auto timestamp = broker_time_point_create(this->timestamp.time_since_epoch().count());

        return broker::message{this->name, this->timestamp.time_since_epoch().count()};
    }
}
