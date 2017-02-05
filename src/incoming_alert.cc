/* incoming_alert.cc
 * ACU Framework
 *
 * <include/acu/incoming_alert.h>
 */

#include "acu/incoming_alert.h"

namespace acu {

    IncomingAlert::IncomingAlert(const std::string *topic, const broker::message &msg) : topic(topic), message(msg) {
        /* The message format uses a mandatory record as the second field of the message
         * which contains 5 fields which are present in every alert. */

        // Require a valid topic
        if (topic == nullptr || topic->empty()) {
            throw new std::invalid_argument("topic not set");
        }

        // Require at least one field
        if (message.size() < 2) {
           throw new std::invalid_argument("msg to small");
        }

        // Require this field to be a broker::record with exactly 5 items
        if (!broker::is<broker::record>(message[1])) {
            throw new std::invalid_argument("msg not a broker::record");
        }
        auto rec = broker::get<broker::record>(message[1]);
        if (rec->size() != 5) {
            throw new std::invalid_argument("msg record too small");
        }
    }

    time_point<system_clock> IncomingAlert::timestamp() const {
        auto rec = broker::get<broker::record>(message[1]);
        assert(broker::is<broker::time_point>(rec->get(0).get()));
        auto raw_duration = broker::get<broker::time_point>(rec->get(0).get())->value;
        auto duration = duration<double, std::ratio<1>>{raw_duration};
        // We need to cast to a duration type with more precision so we get some slightly
        // different values than the ones that were used to create the time_point
        return time_point<system_clock> {duration_cast<system_clock::duration>(duration)};
    }

    const std::string& IncomingAlert::source_ip() const {
        auto rec = broker::get<broker::record>(message[1]);
        assert(broker::is<std::string>(rec->get(1).get()));
        return *broker::get<std::string>(rec->get(1).get());
    }

    const port_t& IncomingAlert::source_port() const {
        auto rec = broker::get<broker::record>(message[1]);
        // Broker only knows one uint type which translates to uint64_t
        assert(broker::is<uint64_t >(rec->get(2).get()));
        return (port_t&)(*broker::get<uint64_t>(rec->get(2).get()));
    }

    const std::string& IncomingAlert::destination_ip() const {
        auto rec = broker::get<broker::record>(message[1]);
        assert(broker::is<std::string>(rec->get(3).get()));
        return *broker::get<std::string>(rec->get(3).get());
    }

    const port_t& IncomingAlert::destination_port() const {
        auto rec = broker::get<broker::record>(message[1]);
        assert(broker::is<uint64_t>(rec->get(4).get()));
        return (port_t&)(*broker::get<uint64_t>(rec->get(4).get()));
    }

    bool IncomingAlert::operator==(const IncomingAlert &rhs) const {
        return message == rhs.message;
    }

    bool IncomingAlert::operator!=(const IncomingAlert &rhs) const {
        return !(*this == rhs);
    }

    std::ostream &operator<<(std::ostream &stream, const IncomingAlert &alert) {
        return stream << *alert.topic << ":" + broker::to_string(alert.message);
    }
}
