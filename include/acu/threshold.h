//
// Created by florian on 12/1/16.
//

#ifndef ACU_FW_TRESHOLD_H
#define ACU_FW_TRESHOLD_H

#include <string>

namespace acu {

    struct Threshold {
    public:
        int count;
        std::string field_name;
        std::string value;

        bool operator==(const Threshold& rhs);
        bool operator!=(const Threshold& rhs);
    };
}


#endif //ACU_FW_TRESHOLD_H
