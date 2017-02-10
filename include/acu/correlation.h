/* correlation.h
 * ACU Framework
 *
 * The correlation algorithms are doing combining, possibly ressource heavy
 * operations on received messages to generate meta alerts.
 *
 * @author: 1wilkens, 0ortmann
 */

#ifndef ACU_FW_CORRELATION_H
#define ACU_FW_CORRELATION_H

#include "outgoing_alert.h"
#include "storage.h"
#include "threshold.h"
#include "utils.h"

namespace acu {

    class Correlation {
    public:
        /// Instantiates a new Correlation.
        ///
        /// \param storage      The shared storage object
        /// \param thresholds   The thresholds for this internal triggers
        Correlation(Storage *storage, std::vector<Threshold> *thresholds)
                : storage(storage), thresholds(thresholds) {};

        /// The storage object, shared with all aggregations and correlations
        Storage *storage;
        /// The thresholds for triggering correlations
        std::vector<Threshold> *thresholds;

        /// Method triggered by aggregation.
        /// The actual correlation will be done in this method.
        virtual OutgoingAlert* Invoke() = 0;
    };
}


#endif //ACU_FW_CORRELATION_H
