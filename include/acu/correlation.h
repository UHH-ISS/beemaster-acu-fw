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
        Correlation(Storage *storage, std::vector<Threshold> *thresholds)
                : storage(storage), thresholds(thresholds) {};

        Storage *storage;
        std::vector<Threshold> *thresholds;

        virtual OutgoingAlert* Invoke() = 0;
    };
}


#endif //ACU_FW_CORRELATION_H
