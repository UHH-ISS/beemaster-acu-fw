//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_AGGREGATIONALGORITHM_H
#define ACU_FW_AGGREGATIONALGORITHM_H

#include "threshold.h"
#include "incoming_alert.h"
#include "storage.h"

namespace acu {

    class Aggregation{
    public:
        Aggregation(Storage *storage, Threshold *thresholds[])
            : storage(storage), thresholds(thresholds) {};

        Storage *storage;
        Threshold **thresholds;

        virtual void Invoke(IncomingAlert *alert) = 0;
    };
}

#endif //ACU_FW_AGGREGATIONALGORITHM_H
