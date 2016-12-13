//
// Created by florian on 12/1/16.
//

#include "acu/threshold.h"

namespace acu {

    bool Threshold::operator==(const Threshold &rhs) const {
        return count == rhs.count
               && field_name == rhs.field_name
               && value == rhs.value;
    }

    bool Threshold::operator!=(const Threshold &rhs) const {
        return !(*this == rhs);
    }
}
