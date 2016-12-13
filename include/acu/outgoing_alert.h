//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_OUTGOING_ALERT_H
#define ACU_FW_OUTGOING_ALERT_H

#include <broker/message.hh>
#include <chrono>
#include <string>

namespace acu {
    using namespace std::chrono;

    class OutgoingAlert {
    public:
        OutgoingAlert(std::string name, time_point<system_clock> timestamp)
                : incidentName(name), timestamp(timestamp) {};

        std::string incidentName;
        time_point<system_clock> timestamp;

        virtual const broker::message ToMessage();
    };
}


#endif //ACU_FW_OUTGOING_ALERT_H
