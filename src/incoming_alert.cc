//
// Created by florian on 12/1/16.
//

#include "acu/incoming_alert.h"

namespace acu {

    IncomingAlert::IncomingAlert(const broker::message &msg) : message(msg) {
        // We need at least 7 items since we directly index into the message
        assert(msg.size() >= 7);
    }

    time_point<system_clock> IncomingAlert::timestamp() {
        assert(broker::is<broker::time_point>(message.at(0)));
        auto val = broker::get<broker::time_point>(message.at(0))->value;
        auto dur = duration<double, std::ratio<1>>{val};
        //TODO: I KNOW this is system_clock.time_since_epoch cast to duration<double>
        // but I cannot figure out how to construct the time_point from this
        // Also ideally this would return a reference but I don't think this is possible
        // See also: https://github.com/bro/broker/blob/master/src/time_point.cc#L8
        // and: http://en.cppreference.com/w/cpp/chrono/time_point
        return system_clock::now();
    }

    std::string& IncomingAlert::incident_type() {
        assert(broker::is<std::string>(message.at(1)));
        return *broker::get<std::string>(message.at(1));
    }

    std::string& IncomingAlert::protocol() {
        assert(broker::is<std::string>(message.at(2)));
        return *broker::get<std::string>(message.at(2));
    }

    std::string& IncomingAlert::source_ip() {
        assert(broker::is<std::string>(message.at(3)));
        return *broker::get<std::string>(message.at(3));
    }

    port_t& IncomingAlert::source_port() {
        assert(broker::is<uint16_t >(message.at(4)));
        return *broker::get<uint16_t>(message.at(4));
    }

    std::string& IncomingAlert::destination_ip() {
        assert(broker::is<std::string>(message.at(5)));
        return *broker::get<std::string>(message.at(5));
    }

    port_t& IncomingAlert::destination_port() {
        assert(broker::is<uint16_t>(message.at(6)));
        return *broker::get<uint16_t>(message.at(6));
    }

    bool IncomingAlert::operator==(const IncomingAlert &rhs) const {
        return message == rhs.message;
    }

    bool IncomingAlert::operator!=(const IncomingAlert &rhs) const {
        return !(*this == rhs);
    }
}
