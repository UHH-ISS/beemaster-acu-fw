//
// Created by florian on 12/1/16.
//

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
