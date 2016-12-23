/* alert_mapper.cc
 * ACU Framework
 * 
 * <include/acu/alert_mapper.h>
 */

#include "acu/alert_mapper.h"

namespace acu {

    IncomingAlert* AlertMapper::GetAlert(const std::string topic, const broker::message &msg) const {
        // This is useful anyways and prevents unused-parameter warnings
        assert(!topic.empty());
        return new IncomingAlert(msg);
    }
}
