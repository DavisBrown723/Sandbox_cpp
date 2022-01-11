#include "pch.h"

#include "SQF.h"

namespace sandbox {
    namespace common {

        namespace convert {

            template<typename T>
            std::vector<T> toArray(intercept::types::game_value value) {
                auto arr = value.to_array();
                return std::vector<T>(arr.begin(), arr.end());
            }

        }

    }
}