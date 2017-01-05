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
        Aggregation(Storage *storage, std::vector<Threshold> *thresholds)
            : storage(storage), thresholds(thresholds) {};

        Storage *storage;
        std::vector<Threshold> *thresholds;

        virtual bool Invoke(const IncomingAlert *alert) = 0;
    };
}

#endif //ACU_FW_AGGREGATIONALGORITHM_H
