//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_CORRELATION_H
#define ACU_FW_CORRELATION_H

#include "threshold.h"
#include "storage.h"

namespace acu {

    class Correlation {
    public:
        Correlation(Storage *storage, Threshold *thresholds[], std::string *topics[])
                : storage(storage), thresholds(thresholds), topics(topics) {};

        Storage *storage;
        Threshold **thresholds;
        std::string **topics;

        virtual void Invoke() = 0;
    };
}


#endif //ACU_FW_CORRELATION_H
