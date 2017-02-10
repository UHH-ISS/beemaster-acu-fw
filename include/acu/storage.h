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
        /// Instantiate the storage object, used for interaction
        /// with the underlying database.
        ///
        /// \param db_name      The name of the database
        Storage(std::string db_name) : db_name(db_name) {}

        /// Name of the database
        std::string db_name;
        // TODO: add more DB properties

        /// Persists the given alert.
        ///
        /// \param alert    The IncomingAlert to put into the DB
        virtual void Persist(const IncomingAlert *alert) = 0;
    };
}


#endif //ACU_FW_STORAGE_H
