//
// Created by flx on 18.12.16.
//

#include "acu/alert_mapper.h"

namespace acu {

    IncomingAlert* AlertMapper::GetAlert(const std::string *topic, const broker::message &msg) const {
        return new IncomingAlert(topic, msg);
    }
}
