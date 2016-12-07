//
// Created by florian on 12/1/16.
//

#include "acu/threshold.h"

bool acu::Threshold::operator==(const acu::Threshold &rhs) {
    return count == rhs.count
        && field_name == rhs.field_name
        && value == rhs.value;
}

bool acu::Threshold::operator!=(const Threshold &rhs) {
    return !(*this == rhs);
}