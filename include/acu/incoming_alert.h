//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_INCOMING_ALERT_H
#define ACU_FW_INCOMING_ALERT_H

#include "utils.h"

#include <broker/message.hh>
#include <chrono>
#include <string>

namespace acu {
    using namespace std::chrono;

    class IncomingAlert {
    public:
        explicit IncomingAlert(const broker::message&);

        // Timestamp indicating when the alert occurred
        time_point<system_clock> timestamp();

        // Incident type (free text)
        std::string& incident_type();
        // Protocol of the incident
        //TODO: Is this tcp/udp or ftp/http/etc?
        std::string& protocol();

        // Source IP of the connection that triggered this alert
        std::string& source_ip();
        // Source port of the connection that triggered this alert
        port_t& source_port() const;

        // Destination IP of the connection that triggered this alert
        std::string& destination_ip();
        // Destination port of the connection that triggered this alert
        port_t& destination_port() const;

        virtual bool operator==(const IncomingAlert&) const;
        virtual bool operator!=(const IncomingAlert&) const;

    protected:
        std::vector<broker::data> message;
    };
} // namespace acu

#endif //ACU_FW_INCOMING_ALERT_H
