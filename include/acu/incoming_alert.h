//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_INCOMING_ALERT_H
#define ACU_FW_INCOMING_ALERT_H

#include <chrono>
#include <string>

#include <broker/message.hh>

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
        // TODO: Change this to port type when utils.h is merged
        // TODO: Also do we want to return &uint16_t here to match the other "fields"?
        // Source port of the connection that triggered this alert
        uint16_t source_port();

        // Destination IP of the connection that triggered this alert
        std::string& destination_ip();
        // TODO: Change this to port type when utils.h is merged
        // Destination port of the connection that triggered this alert
        uint16_t destination_port();

        virtual bool operator==(const IncomingAlert&) const;
        virtual bool operator!=(const IncomingAlert&) const;

    protected:
        std::vector<broker::data> message;

    private:
        IncomingAlert() {};
    };
} // namespace acu

#endif //ACU_FW_INCOMING_ALERT_H
