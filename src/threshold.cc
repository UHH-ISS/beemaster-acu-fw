/* threshold.cc
 * ACU Framework
 *
 * <include/acu/threshold.h>
 */

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
