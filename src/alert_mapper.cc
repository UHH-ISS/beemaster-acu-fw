//
// Created by flx on 18.12.16.
//

#include "acu/alert_mapper.h"

namespace acu {

    IncomingAlert* AlertMapper::GetAlert(const std::string topic, const broker::message &msg) const {
        // This is useful anyways and prevents unused-parameter warnings
        assert(!topic.empty());
        return new IncomingAlert(msg);
    }
}
