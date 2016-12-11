//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_OUTGOING_ALERT_H
#define ACU_FW_OUTGOING_ALERT_H

#include <chrono>
#include <string>
#include <broker/message.hh>

namespace acu {

    class OutgoingAlert {
    public:
        OutgoingAlert(std::string name, std::chrono::time_point<std::chrono::system_clock> timestamp)
                : name(name), timestamp(timestamp) {};

        std::string name;
        std::chrono::time_point<std::chrono::system_clock> timestamp;

        virtual const broker::message AsMessage();
    };
}


#endif //ACU_FW_OUTGOING_ALERT_H
