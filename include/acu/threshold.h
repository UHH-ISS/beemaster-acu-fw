/* threshold.h
 * ACU Framework
 *
 * The threshold is mainly a trigger-value for correlation and aggregation
 * algorithms. It is a simple, efficient data struct.
 *
 * @author: 1wilkens
 */

#ifndef ACU_FW_THRESHOLD_H
#define ACU_FW_THRESHOLD_H

#include <string>

namespace acu {

    struct Threshold {
        Threshold(int count, std::string field_name, std::string value)
                : count(count), field_name(field_name), value(value) {}

        // Count after the threshold is reached
        uint64_t count;
        // Field name which should be checked for the value
        std::string field_name;
        // Value of the field which will be counted
        std::string value;

        bool operator==(const Threshold&) const;
        bool operator!=(const Threshold&) const;
    };
} // namespace acu

namespace std {

    template <> struct hash<acu::Threshold> {
        inline size_t operator()(const acu::Threshold &t) const {
            return hash<int>()(t.count) ^ hash<string>()(t.field_name) ^ hash<string>()(t.value);
        }
    };
} // namespace std


#endif //ACU_FW_THRESHOLD_H
