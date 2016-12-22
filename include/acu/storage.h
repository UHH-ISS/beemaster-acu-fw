/* storage.h
 * ACU Framework
 *
 * A simple abstract interface to the storage (database).
 *
 * @author: 0ortmann
 */

#ifndef ACU_FW_STORAGE_H
#define ACU_FW_STORAGE_H

#include "incoming_alert.h"

namespace acu {

    class Storage {
    public:
        Storage(std::string db_name) : db_name(db_name) {}

        std::string db_name;
        // TODO: add more DB properties

        virtual void Persist(IncomingAlert *alert) = 0;
    };
}


#endif //ACU_FW_STORAGE_H
