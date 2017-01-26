/* receiver.h
 * ACU Framework
 *
 * The receiver peers with the given site and listens on incoming messages. The
 * given callback will be called upon receiving such a message.
 *
 * @author: 1wilkens, 0ortmann, 1jost
 */

#ifndef ACU_FW_RECEIVER_H
#define ACU_FW_RECEIVER_H

#include "alert_mapper.h"
#include "utils.h"

#include <broker/endpoint.hh>
#include <broker/message.hh>
#include <functional>
#include <queue>
#include <string>
#include <vector>


namespace acu {

    class Receiver {
    public:
        /// Initialise a new Receiver.
        ///
        /// \param address  The address to listen on.
        /// \param port     The port to listen on.
        /// \param topics   The topics to subscribe to.
        Receiver(std::string address, port_t port, std::vector<std::string>* topics, AlertMapper *mapper)
                : address(address), port(port), topics(topics), mapper(mapper) {};

        // TODO: The broker message could/should be a reference to transfer ownership to the acu
        void Peer(std::queue<IncomingAlert*>*);

    private:
        std::string address;
        port_t port;
        std::vector<std::string> *topics;
        AlertMapper *mapper;
    };

    // This could also be set via config file?
    const static std::string ENDPOINT_NAME = "acu_receiver";
}


#endif //ACU_FW_RECEIVER_H
