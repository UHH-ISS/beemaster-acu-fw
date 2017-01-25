/* incoming_alert.cc
 * ACU Framework
 *
 * <include/acu/incoming_alert.h>
 */

#include "acu/incoming_alert.h"

namespace acu {

    IncomingAlert::IncomingAlert(const std::string *topic, const broker::message &msg) : topic(topic), message(msg) {
        /* The message format uses a mandatory record as the first field of the message
         * which contains 5 fields which are present in every alert. */

        // Require at least one field
        assert(!topic->empty());
        assert(msg.size() >= 1);

        // Require this field to be a broker::record with exactly 5 items
        assert(broker::is<broker::record>(message[0]));
        auto rec = broker::get<broker::record>(message[0]);
        assert(rec->size() == 5);
        // TODO: We could also "typecheck" the message fields here to fail early?
    }

    time_point<system_clock> IncomingAlert::timestamp() {
        auto rec = broker::get<broker::record>(message[0]);
        assert(broker::is<broker::time_point>(rec->get(0).get()));
        auto val = broker::get<broker::time_point>(rec->get(0).get())->value;
        auto dur = duration<double, std::ratio<1>>{val};
        //TODO: I KNOW this is system_clock.time_since_epoch cast to duration<double>
        // but I cannot figure out how to construct the time_point from this
        // Also ideally this would return a reference but I don't think this is possible
        // See also: https://github.com/bro/broker/blob/master/src/time_point.cc#L8
        // and: http://en.cppreference.com/w/cpp/chrono/time_point
        return system_clock::now();
    }

    const std::string& IncomingAlert::source_ip() const {
        auto rec = broker::get<broker::record>(message[0]);
        assert(broker::is<std::string>(rec->get(1).get()));
        return *broker::get<std::string>(rec->get(1).get());
    }

    const port_t& IncomingAlert::source_port() const {
        auto rec = broker::get<broker::record>(message[0]);
        // Broker only knows one uint type which translates to uint64_t
        assert(broker::is<uint64_t >(rec->get(2).get()));
        return (port_t&)(*broker::get<uint64_t>(rec->get(2).get()));
    }

    const std::string& IncomingAlert::destination_ip() const {
        auto rec = broker::get<broker::record>(message[0]);
        assert(broker::is<std::string>(rec->get(3).get()));
        return *broker::get<std::string>(rec->get(3).get());
    }

    const port_t& IncomingAlert::destination_port() const {
        auto rec = broker::get<broker::record>(message[0]);
        assert(broker::is<uint64_t>(rec->get(4).get()));
        return (port_t&)(*broker::get<uint64_t>(rec->get(4).get()));
    }

    bool IncomingAlert::operator==(const IncomingAlert &rhs) const {
        return message == rhs.message;
    }

    bool IncomingAlert::operator!=(const IncomingAlert &rhs) const {
        return !(*this == rhs);
    }
}
