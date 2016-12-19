//
// Created by flx on 18.12.16.
//

#ifndef ACU_FW_ALERT_MAPPER_H
#define ACU_FW_ALERT_MAPPER_H

#include "incoming_alert.h"

namespace acu {

    class AlertMapper {

    public:
        AlertMapper() {};

        IncomingAlert* GetAlert(std::string topic, const broker::message &msg) {
            return nullptr;
        };

    };
}
#endif //ACU_FW_ALERT_MAPPER_H
