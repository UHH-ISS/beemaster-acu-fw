//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_THRESHOLD_H
#define ACU_FW_THRESHOLD_H

#include <string>

namespace acu {

    struct Threshold {
        Threshold(int count, std::string field_name, std::string value)
                : count(count), field_name(field_name), value(value) {}

        // Count after the threshold is reached
        int count;
        // Field name which should be checked for the value
        std::string field_name;
        // Value of the field which will be counted
        std::string value;

        bool operator==(const Threshold &rhs) const;
        bool operator!=(const Threshold &rhs) const;
    };
} // namespace acu

namespace std {

    template <> struct hash<acu::Threshold> {
        inline size_t operator()(const acu::Threshold& t) const {
            return hash<int>()(t.count) ^ hash<string>()(t.field_name) ^ hash<string>()(t.value);
        }
    };
} // namespace std


#endif //ACU_FW_THRESHOLD_H
