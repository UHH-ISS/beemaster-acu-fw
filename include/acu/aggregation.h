//
// Created by florian on 12/1/16.
//

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

        virtual bool Invoke(IncomingAlert *alert) = 0;
    };
}

#endif //ACU_FW_AGGREGATIONALGORITHM_H
