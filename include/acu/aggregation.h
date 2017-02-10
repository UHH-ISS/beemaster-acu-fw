/* aggregation.h
 * ACU Framework
 *
 * The aggregation algorithms are rather simple algorithms, meant to collect
 * information from the incoming alerts to eventually trigger the correlation
 * algorithms.
 *
 * @author: 1wilkens, 0ortmann
 */

#ifndef ACU_FW_AGGREGATIONALGORITHM_H
#define ACU_FW_AGGREGATIONALGORITHM_H

#include "incoming_alert.h"
#include "storage.h"
#include "threshold.h"

namespace acu {

    class Aggregation {
    public:
        /// Instantiates a new Aggregation.
        ///
        /// \param storage      The shared storage object
        /// \param thresholds   The thresholds for this aggregation to trigger correlations
        Aggregation(Storage *storage, std::vector<Threshold> *thresholds)
            : storage(storage), thresholds(thresholds) {};

        /// The storage object, shared with all aggregations and correlations
        Storage *storage;
        /// The thresholds for triggering correlations
        std::vector<Threshold> *thresholds;

        /// Method called, when a new IncomingAlert is available.
        /// The actual aggregation will be done by this method.
        ///
        /// \param alert    The new IncomingAlert
        /// \return         True if correlation shall be triggered,
        ///                 false otherwise
        virtual bool Invoke(const IncomingAlert *alert) = 0;
    };
}

#endif //ACU_FW_AGGREGATIONALGORITHM_H
